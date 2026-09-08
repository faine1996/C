#include "menu.h"
#include "submarine.h"
#include "combatsubmarine.h"
#include "researchsubmarine.h"
#include "centralcomputer.h"
#include "message.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

/* -----------------------------------------------------------------------
 * Test framework
 * --------------------------------------------------------------------- */

static int s_passed  = 0;
static int s_failed  = 0;
static int s_skipped = 0;

static void check(const string &test_name,
                  const string &output,
                  const string &expected_fragment)
{
    if (output.find(expected_fragment) != string::npos)
    {
        cout << "[PASS] " << test_name << "\n";
        ++s_passed;
    }
    else
    {
        cout << "[FAIL] " << test_name << "\n";
        cout << "       Expected to find: \"" << expected_fragment << "\"\n";
        cout << "       In output:\n" << output << "\n";
        ++s_failed;
    }
}

static void skip(const string &test_name, const string &reason)
{
    cout << "[SKIP] " << test_name << " — " << reason << "\n";
    ++s_skipped;
}

static string run_menu(const vector<string> &input_lines)
{
    string combined;

    for (const string &line : input_lines)
    {
        combined += line + "\n";
    }

    combined += "10\n";

    istringstream input_stream(combined);
    ostringstream output_stream;

    streambuf *old_cin  = cin.rdbuf(input_stream.rdbuf());
    streambuf *old_cout = cout.rdbuf(output_stream.rdbuf());

    {
        Menu menu;
        menu.run();
    }

    cin.rdbuf(old_cin);
    cout.rdbuf(old_cout);

    return output_stream.str();
}

/* -----------------------------------------------------------------------
 * Part 1 — Menu and OOP tests
 * --------------------------------------------------------------------- */

static void test_add_research_submarine()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology"
    });

    check("Add research submarine", out, "Research submarine added");
}

static void test_add_first_combat_submarine()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null"
    });

    check("Add first combat submarine — no live link", out, "no live link");
}

static void test_add_second_combat_no_port_prompt()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "1", "CS-002", "Shark",     "2", "Jones",  "8", "/dev/null",
        "2"
    });

    check("Second combat sub added", out, "Combat submarine added");
}

static void test_display_all_empty()
{
    string out = run_menu({ "2" });
    check("Display all — empty fleet", out, "Fleet is empty");
}

static void test_display_all_populated()
{
    string out = run_menu({
        "1", "RS-001", "Neptune",   "1", "Deep sea geology",
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "2"
    });

    check("Display all — shows research sub",  out, "Neptune");
    check("Display all — shows combat sub",    out, "Barracuda");
    check("Display all — shows live link: no", out, "Live link : no");
}

static void test_find_by_serial_found()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology",
        "3", "RS-001"
    });

    check("Find by serial — found", out, "Neptune");
}

static void test_find_by_serial_not_found()
{
    string out = run_menu({ "3", "XX-999" });
    check("Find by serial — not found", out, "not found");
}

static void test_assign_mission_research()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology",
        "4", "RS-001", "Hydrothermal vent survey"
    });

    check("Assign mission — research sub", out, "Mission assigned");
}

static void test_assign_mission_already_assigned()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology",
        "4", "RS-001", "First mission",
        "4", "RS-001"
    });

    check("Assign mission — already on mission", out, "already on a mission");
}

static void test_assign_mission_combat()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "4", "CS-001", "Northern patrol"
    });

    check("Assign mission — combat sub", out, "Mission assigned");
}

static void test_update_mission_research()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology",
        "4", "RS-001", "Initial topic",
        "5", "RS-001", "Bioluminescent organisms"
    });

    check("Update mission — research topic", out, "Topic updated");
}

static void test_update_mission_not_on_mission()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "5", "CS-001"
    });

    check("Update mission — not on mission", out, "not currently on a mission");
}

static void test_update_mission_combat_description()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "5", "CS-001", "1", "Northern patrol extended"
    });

    check("Update mission — combat description", out, "Mission description updated");
}

static void test_update_mission_combat_commander()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "5", "CS-001", "2", "Williams"
    });

    check("Update mission — combat commander", out, "Commander updated");
}

static void test_end_mission()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "6", "CS-001"
    });

    check("End mission", out, "Mission ended");
}

static void test_end_mission_not_on_mission()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "6", "CS-001"
    });

    check("End mission — not on mission", out, "not on a mission");
}

static void test_mission_history_persists()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "4", "CS-001", "First patrol",
        "6", "CS-001",
        "4", "CS-001", "Second patrol",
        "6", "CS-001",
        "2"
    });

    check("Mission history — first entry",  out, "First patrol");
    check("Mission history — second entry", out, "Second patrol");
}

static void test_add_companion()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "1", "CS-002", "Shark",     "2", "Jones",  "8", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "4", "CS-002", "Northern patrol",
        "7", "CS-001", "CS-002"
    });

    check("Add companion", out, "Companion added");
}

static void test_add_companion_duplicate()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "1", "CS-002", "Shark",     "2", "Jones",  "8", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "4", "CS-002", "Northern patrol",
        "7", "CS-001", "CS-002",
        "7", "CS-001", "CS-002"
    });

    check("Add companion — duplicate", out, "Already a companion");
}

static void test_add_companion_self()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "7", "CS-001", "CS-001"
    });

    check("Add companion — self", out, "cannot be its own companion");
}

static void test_add_companion_not_combat()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology",
        "7", "RS-001"
    });

    check("Add companion — not combat sub", out, "not found");
}

static void test_send_message()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "1", "CS-002", "Shark",     "2", "Jones",  "8", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "4", "CS-002", "Northern patrol",
        "7", "CS-001", "CS-002",
        "8", "CS-001", "CS-002", "Rendezvous at grid 7"
    });

    check("Send message", out, "Message sent");
}

static void test_send_message_not_companion()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "1", "CS-002", "Shark",     "2", "Jones",  "8", "/dev/null",
        "8", "CS-001", "CS-002"
    });

    check("Send message — not companion", out, "not a companion");
}

static void test_send_message_to_self()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "8", "CS-001", "CS-001"
    });

    check("Send message — to self", out, "Cannot send a message to yourself");
}

static void test_display_messages()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "1", "CS-002", "Shark",     "2", "Jones",  "8", "/dev/null",
        "4", "CS-001", "Northern patrol",
        "4", "CS-002", "Northern patrol",
        "7", "CS-001", "CS-002",
        "8", "CS-001", "CS-002", "Rendezvous at grid 7",
        "9", "CS-002"
    });

    check("Display messages — content", out, "Rendezvous at grid 7");
    check("Display messages — sender",  out, "Barracuda");
}

static void test_display_messages_empty()
{
    string out = run_menu({
        "1", "CS-001", "Barracuda", "2", "Smith", "12", "/dev/null",
        "9", "CS-001"
    });

    check("Display messages — none", out, "No messages received");
}

static void test_display_messages_research_sub()
{
    string out = run_menu({
        "1", "RS-001", "Neptune", "1", "Deep sea geology",
        "9", "RS-001"
    });

    check("Display messages — research sub", out, "not found");
}

static void test_invalid_menu_choice()
{
    string out = run_menu({ "99" });
    check("Invalid menu choice", out, "Invalid choice");
}

static void test_invalid_submarine_type()
{
    string out = run_menu({ "1", "XX-001", "Ghost", "9" });
    check("Invalid submarine type", out, "Invalid type");
}

/* -----------------------------------------------------------------------
 * Part 2 — Live LNC serial tests (board must be plugged in)
 * --------------------------------------------------------------------- */

static bool port_available(const string &port)
{
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd < 0)
    {
        return false;
    }

    close(fd);
    return true;
}

static void test_serial_port_opens()
{
    const string port = "/dev/ttyACM0";

    if (!port_available(port))
    {
        skip("Serial port opens", "board not connected");
        return;
    }

    CentralComputer cc(port);
    check("Serial port opens", cc.isLive() ? "live" : "", "live");
}

static void test_keepalive_received()
{
    const string port = "/dev/ttyACM0";

    if (!port_available(port))
    {
        skip("KEEPALIVE received", "board not connected");
        return;
    }

    CentralComputer cc(port);

    if (!cc.isLive())
    {
        skip("KEEPALIVE received", "port failed to open");
        return;
    }

    ostringstream captured;
    streambuf *old_cout = cout.rdbuf(captured.rdbuf());

    for (int i = 0; i < 100; ++i)
    {
        cc.processIncoming();
        usleep(100000);
    }

    cout.rdbuf(old_cout);

    check("KEEPALIVE received", captured.str(), "KEEPALIVE");
}

/*
 * TIME_SYNC_REQ and the startup event are both one-shot messages fired
 * once per boot, in sequence (Init_Task sends TIME_SYNC_REQ, waits for
 * SET_TIME, then sends startup). Each fresh CentralComputer connection
 * only gets the bytes still sitting unread on the wire, so whichever
 * connection reads first consumes both — a second, separate connection
 * opened afterward has nothing left to see. Checking both in one shared
 * connection is the only way for both assertions to pass on the same boot.
 */
static void test_startup_and_time_sync_received()
{
    const string port = "/dev/ttyACM0";

    if (!port_available(port))
    {
        skip("Startup event and TIME_SYNC_REQ received", "board not connected");
        return;
    }

    CentralComputer cc(port);

    if (!cc.isLive())
    {
        skip("Startup event and TIME_SYNC_REQ received", "port failed to open");
        return;
    }

    ostringstream captured;
    streambuf *old_cout = cout.rdbuf(captured.rdbuf());

    for (int i = 0; i < 100; ++i)
    {
        cc.processIncoming();
        usleep(100000);
    }

    cout.rdbuf(old_cout);

    string out = captured.str();

    check("Startup event received", out, "startup");
    check("TIME_SYNC_REQ received and SET_TIME sent", out, "SET_TIME sent");
}

/* -----------------------------------------------------------------------
 * Main
 * --------------------------------------------------------------------- */

int main()
{
    cout << "\n=== Part 1: Menu and OOP tests ===\n\n";

    test_add_research_submarine();
    test_add_first_combat_submarine();
    test_add_second_combat_no_port_prompt();
    test_display_all_empty();
    test_display_all_populated();
    test_find_by_serial_found();
    test_find_by_serial_not_found();
    test_assign_mission_research();
    test_assign_mission_already_assigned();
    test_assign_mission_combat();
    test_update_mission_research();
    test_update_mission_not_on_mission();
    test_update_mission_combat_description();
    test_update_mission_combat_commander();
    test_end_mission();
    test_end_mission_not_on_mission();
    test_mission_history_persists();
    test_add_companion();
    test_add_companion_duplicate();
    test_add_companion_self();
    test_add_companion_not_combat();
    test_send_message();
    test_send_message_not_companion();
    test_send_message_to_self();
    test_display_messages();
    test_display_messages_empty();
    test_display_messages_research_sub();
    test_invalid_menu_choice();
    test_invalid_submarine_type();

    cout << "\n=== Part 2: Live LNC serial tests ===\n\n";

    test_serial_port_opens();
    test_startup_and_time_sync_received();
    test_keepalive_received();

    cout << "\n=== Results ===\n";
    cout << "Passed:  " << s_passed  << "\n";
    cout << "Failed:  " << s_failed  << "\n";
    cout << "Skipped: " << s_skipped << "\n";

    return s_failed > 0 ? 1 : 0;
}