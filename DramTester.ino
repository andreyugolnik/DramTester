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

// -----------------------------------------------------------------------------

static void breakWithoutErrors(const cLedsList& ledsList)
{
    CurrentState = State::Idle;

    ledsList.showGreen();

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

static bool breakOnError(const cTest::Result& testResult, const cLedsList& ledsList)
{
    const bool hasErrors = testResult == true;

    if (hasErrors)
    {
        CurrentState = State::Idle;

        ledsList.showRed();

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

int main()
{
    init();

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

    cLedsList ledsList;
    ledsList.setup();
    ledsList.switchOff();

    cButton btnStart(BTN_START);
    btnStart.setup();

    pinMode(DOUT, INPUT);
    pinMode(DIN, OUTPUT);

    pinMode(CAS, OUTPUT);
    pinMode(RAS, OUTPUT);
    pinMode(WRITE, OUTPUT);

    digitalWrite(CAS, HIGH);
    digitalWrite(RAS, HIGH);
    digitalWrite(WRITE, HIGH);

    cTestPlain testPlain(Verbose, AddressBits);
    cTestReversed testReversed(Verbose, AddressBits);
    cTestRandom testRandom(Verbose, AddressBits);

    uint32_t CurrentTestIndex = 0;

    while (true)
    {
        if (CurrentState == State::Testing)
        {
            bool hasErrors = false;

            const TestProp& prop = TestsList[CurrentTestIndex];
            switch (prop.type)
            {
            case TestProp::Type::Plain:
                hasErrors = breakOnError(testPlain.doTest(prop.value, ledsList), ledsList);
                break;

            case TestProp::Type::Reversed:
                hasErrors = breakOnError(testReversed.doTest(prop.value, ledsList), ledsList);
                break;

            case TestProp::Type::Random:
                hasErrors = breakOnError(testRandom.doTest(millis(), ledsList), ledsList);
                break;
            }

            if (hasErrors == false)
            {
                CurrentTestIndex++;

                const uint32_t totalTests = sizeof(TestsList) / sizeof(TestsList[0]);
                Serial.println(String(100.0f * CurrentTestIndex / totalTests) + "%");

                if (CurrentTestIndex == totalTests)
                {
                    breakWithoutErrors(ledsList);
                }
            }
        }
        else
        {
            if (btnStart.update())
            {
                if (CurrentState == State::Idle)
                {
                    CurrentState = State::Testing;
                    CurrentTestIndex = 0;
                    ledsList.showIdle();
                }
            }

            ledsList.update();
            delay(10);
        }
    }

    return 0;
}
