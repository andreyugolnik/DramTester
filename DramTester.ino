/**********************************************\
*
*  Andrey A. Ugolnik
*  http://www.ugolnik.info
*  andrey@ugolnik.info
*
\**********************************************/

#include "src/Button.h"
#include "src/Dram.h"
#include "src/Led.h"
#include "src/LedsList.h"
#include "src/PinsConfig.h"
#include "src/TestPlain.h"
#include "src/TestRandom.h"
#include "src/TestReversed.h"

// -----------------------------------------------------------------------------

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
    { TestProp::Type::Reversed, 1 },
    { TestProp::Type::Plain, 1 },
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

static void readyToTestChipMessage()
{
    Serial.println("");
    Serial.println("o======================================================o");
    Serial.println("|          Insert DRAM and press Start button          |");
    Serial.println("o======================================================o");
    Serial.println("");
    Serial.flush();
}

static void showGoodRamMessage()
{
    Serial.println("");
    Serial.println("              o=========================o");
    Serial.println("              |                         |");
    Serial.println("              |    All tests is done    |");
    Serial.println("              |       DRAM is OK!       |");
    Serial.println("              |                         |");
    Serial.println("              o=========================o");
    readyToTestChipMessage();
}

static void showBadRamMessage()
{
    Serial.println("");
    Serial.println("              o===========================o");
    Serial.println("              |                           |");
    Serial.println("              |    DRAM has BAD cells!    |");
    Serial.println("              |                           |");
    Serial.println("              o===========================o");
    readyToTestChipMessage();
}

// -----------------------------------------------------------------------------

int main()
{
    init();

    Serial.begin(9600);
    while (!Serial)
    {
        _delay_us(100);
    }

    Serial.println("DRAM Tester v0.2 / Feb 20, 2022");
    Serial.println("by Andrey A. Ugolnik");
    Serial.println("");

    cDram dram;

    const char* chipType = "64Kx1";
    if (dram.getAddressBits() == 9)
    {
        chipType = "256Kx1";
    }
    Serial.println("Testing DRAM " + String(chipType)
                   + " with " + String(dram.getAddressBits())
                   + " address line bits");
    readyToTestChipMessage();

    cLedsList ledsList;
    ledsList.switchOff();

    cButton btnStart(BTN_START);

    cTestPlain testPlain(Verbose);
    cTestReversed testReversed(Verbose);
    cTestRandom testRandom(Verbose);

    uint32_t CurrentTestIndex = 0;

    while (true)
    {
        if (CurrentState == State::Testing)
        {
            cTest::Result result = cTest::Result::OK;

            const TestProp& prop = TestsList[CurrentTestIndex];
            switch (prop.type)
            {
            case TestProp::Type::Plain:
                result = testPlain.doTest(prop.value, dram, ledsList);
                break;

            case TestProp::Type::Reversed:
                result = testReversed.doTest(prop.value, dram, ledsList);
                break;

            case TestProp::Type::Random:
                result = testRandom.doTest(millis(), dram, ledsList);
                break;
            }

            if (result == cTest::Result::OK)
            {
                CurrentTestIndex++;

                const uint32_t totalTests = sizeof(TestsList) / sizeof(TestsList[0]);
                Serial.println("  => " + String(100.0f * CurrentTestIndex / totalTests, 1) + "%");

                if (CurrentTestIndex == totalTests)
                {
                    CurrentState = State::Idle;
                    ledsList.showGreen();
                    showGoodRamMessage();
                }
            }
            else
            {
                CurrentState = State::Idle;
                ledsList.showRed();
                showBadRamMessage();
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

            _delay_us(100);
        }
    }

    return 0;
}
