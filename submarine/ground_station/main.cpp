#include "menu.h"

int main(int argc, char *argv[])
{
    std::string host = "127.0.0.1";
    uint16_t    port = 5555;

    if (argc > 1)
    {
        host = argv[1];
    }

    if (argc > 2)
    {
        port = static_cast<uint16_t>(std::stoi(argv[2]));
    }

    Menu menu(host, port);
    menu.run();

    return 0;
}
