/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "src/Button.h"
#include "src/Led.h"
#include "src/PinsConfig.h"
#include "src/TestPlain.h"
#include "src/TestRandom.h"
#include "src/TestReversed.h"

// -----------------------------------------------------------------------------

// PB0..PB5 + PC0..PC2 is used to connect to the address line A0..A8 of the RAM
// Up to 9 bits for address line
// 8 bits for 4164
// 9 bits for 41256
uint32_t AddressBits = 8;

const bool Verbose = false;

// -----------------------------------------------------------------------------

enum class State
{
    Idle,
    Testing
};

static State CurrentState = State::Idle;

struct TestProp
{
    enum class Type
    {
        Plain,
        Reversed,
        Random,
    };

    Type type;
    uint32_t value;
};

static const TestProp TestsList[] = {
    { TestProp::Type::Plain, 0 },
    { TestProp::Type::Plain, 1 },
    { TestProp::Type::Reversed, 1 },
    { TestProp::Type::Reversed, 0 },
    { TestProp::Type::Random, 0 },
    { TestProp::Type::Random, 0 },
    { TestProp::Type::Plain, 1 },
    { TestProp::Type::Plain, 0 },
    { TestProp::Type::Reversed, 0 },
    { TestProp::Type::Reversed, 1 },
    { TestProp::Type::Random, 0 },
    { TestProp::Type::Random, 0 },
};

static uint32_t CurrentTestIndex = 0;

static cLedsList LedsList;

static cTestPlain TestPlain(Verbose, AddressBits);
static cTestReversed TestReversed(Verbose, AddressBits);
static cTestRandom TestRandom(Verbose, AddressBits);

static cButton BtnStart(BTN_START);

// -----------------------------------------------------------------------------

static void breakWithoutErrors()
{
    CurrentState = State::Idle;

    LedsList.showGreen();

    Serial.println("");
    Serial.println("--------------------------------------------------------");
    Serial.println("All tests is done.");
    Serial.println("DRAM is OK!");
    Serial.println("--------------------------------------------------------");
    Serial.println("");
    Serial.println("Insert another chip and press Start button");
    Serial.println("");
    Serial.println("");
    Serial.flush();
}

static bool breakOnError(const cTest::Result& testResult)
{
    const bool hasErrors = testResult == true;

    if (hasErrors)
    {
        CurrentState = State::Idle;

        LedsList.showRed();

        Serial.println("FAILED at row "
                       + String(testResult.row)
                       + ", col "
                       + String(testResult.col)
                       + ", was expecting "
                       + String(testResult.val)
                       + " got "
                       + String(!testResult.val)
                       + ".");
        Serial.println("--------------------------------------------------------");
        Serial.println("");
        Serial.println("--------------------------------------------------------");
        Serial.println("DRAM has BAD cells!");
        Serial.println("--------------------------------------------------------");
        Serial.println("");
        Serial.println("Insert another chip and press Start button");
        Serial.println("");
        Serial.println("");
        Serial.flush();
    }

    return hasErrors;
}

// -----------------------------------------------------------------------------

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }

    Serial.println("DRAM Tester v0.2 / Feb 20, 2022");
    Serial.println("by Andrey A. Ugolnik");
    Serial.println("");

    const char* chipType = "64Kx1";
    if (AddressBits == 9)
    {
        chipType = "256Kx1";
    }
    Serial.println("Testing DRAM " + String(chipType)
                   + " with " + String(AddressBits)
                   + " address line bits");
    Serial.println("");
    Serial.println("Insert chip and press Start button");
    Serial.println("");
    Serial.flush();

    // randomSeed(analogRead(LED_G));

    // reserve 9 bits for address line
    DDRB = 0x3f; // PB0..PB5 0b00111111
    DDRC = 0x07; // PC0..PC2 0b00000111

    LedsList.setup();
    LedsList.switchOff();

    BtnStart.setup();

    pinMode(DOUT, INPUT);
    pinMode(DIN, OUTPUT);

    pinMode(CAS, OUTPUT);
    pinMode(RAS, OUTPUT);
    pinMode(WRITE, OUTPUT);

    digitalWrite(CAS, HIGH);
    digitalWrite(RAS, HIGH);
    digitalWrite(WRITE, HIGH);
}

void loop()
{
    if (CurrentState == State::Testing)
    {
        bool hasErrors = false;

        const TestProp& prop = TestsList[CurrentTestIndex];
        switch (prop.type)
        {
        case TestProp::Type::Plain:
            hasErrors = breakOnError(TestPlain.doTest(prop.value, LedsList));
            break;

        case TestProp::Type::Reversed:
            hasErrors = breakOnError(TestReversed.doTest(prop.value, LedsList));
            break;

        case TestProp::Type::Random:
            hasErrors = breakOnError(TestRandom.doTest(millis(), LedsList));
            break;
        }

        if (hasErrors == false)
        {
            CurrentTestIndex++;

            const uint32_t totalTests = sizeof(TestsList) / sizeof(TestsList[0]);
            Serial.println(String(100.0f * CurrentTestIndex / totalTests) + "%");

            if (CurrentTestIndex == totalTests)
            {
                breakWithoutErrors();
            }
        }
    }
    else
    {
        if (BtnStart.update())
        {
            if (CurrentState == State::Idle)
            {
                CurrentState = State::Testing;
                CurrentTestIndex = 0;
                LedsList.showIdle();
            }
        }

        LedsList.update();
        delay(10);
    }
}
