// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials 
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "timer.h"
extern u32int placement_address;
u32int initial_esp;

// higher number = slower scroll
#define SCROLLTIME 4000
void cancer_intro();
void real_intro();

int main(struct multiboot *mboot_ptr, u32int initial_stack) {
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    
    // Initialise the PIT to 100Hz
    asm volatile("sti");
    init_timer(50);

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);

    // kek
    cancer_intro();
    real_intro();

    // Create a new process in a new address space which is a clone of this.
    int ret = fork();

    monitor_write("fork() returned ");
    monitor_write_hex(ret);
    monitor_write(", and getpid() returned ");
    monitor_write_hex(getpid());
    monitor_write("\n============================================================================\n");

    // The next section of code is not reentrant so make sure we aren't interrupted during.
    asm volatile("cli");
    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0) {
        monitor_write("Found file ");
        monitor_write(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags&0x7) == FS_DIRECTORY) {
            monitor_write("\n\t(directory)\n");
        } else {
            monitor_write("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                monitor_put(buf[j]);
            
            monitor_write("\"\n");
        }
        i++;
    }

    asm volatile("sti");

    return 0;

}

void pause() {
    for (int i = 0; i < 1000 * SCROLLTIME; i++);
}

void cancer_intro() {
    char *wot = 
"                               ````...-::///+++++//::--.````                   \n" "                           ```````````....------:::--.....```                  \n" "                       ````````````````...`````````.....```````                \n" "                 ```...-.````````..````````...`````````````````                \n" "              ```.-:+++:.````````....-.........```````  ````````````           \n" "            `  ``.-///:.`` ```````````.....````````````` ````````````          \n" "            ````.-:::-```````  ```````````````````````````````````````         \n" "          ``````.-:/-...``         `       ```````````````````````  ```        \n" "       ````````.://+/-:-.``                  `````````````````````             \n" "       ```````..-:///-..```                        ```````````````             \n" "   `   ````...----:-.`````                           `  ````````` ```          \n" "  ``     ```.......``````  `                        ```````````````````        \n" "        `   ``````` `````    ```                   ``` ``````````````          \n" " `             ```````````  ````````             ````````````````````` ``      \n" "``            ``````````````````````````   ```` `````````````````````     `    \n" "``           `..-----..`````.--.........``````````````````````````````         \n" "            `.--::////::--...-::-------:---......```````````````````` ```   `  \n" "           `.-::///+++++++++////+++//::://////:--...`````````````````  ```  `` \n" "          `.-:///+++oooooooooooooooossooooooooo++/:-..``````````````   ``````` \n" "        ```-://+++oooooooosssssssssssyyyyyyyysssso+/:-.``` ```.``````````````  \n" "        ``.-///+++ooooooossssssyyyyyyyyyyyyyyyyyyssso+/-.````....``````````  ``\n" "       ```.://++++oooooosssssyyyyyyyyyhhhhyyyyyyyyyyyyso/-```-:::-.`````````` `\n" "   `   ``.-://++++oooossssyyyyyyyyyyyhhhhhhhhhhhhhyyyyysso/-..:///:.```````` ``\n" "``     ``.-://+++ooosssyyyyyyhhhhhhhhhhhhhhhhdddhhhhhyssssso+++++o+:.```  `````\n" "``     ``.-://+///+++oosyyyyhhhhhhhhhhhhhhhhhyssooooosoooosssoooooo+-` ````````\n" "`       `.:://////////////+osyyyyhhhhdhhhyysoooooosssyyyssssossooooo:` ````````\n" "        `-://+++ooossssyssssssysyyyhhddhhyyyyyssssyyyyyyyyyysyssoooo/. ` ``````\n" "``      .-:/++ooooossssyyyyyyysoossyhhhhhhyysssoo++oosssssyyyyysssso/.   ``````\n" "        .:/+++++/://---::++ssso++ossyyyhhhyso++s/....:/:/+osssyyssso+-`` ``````\n" "      ` -//++/::-:/+/:/+osoooo+++osyyyyyyyysooossooooooooooosyyyssso+/``  ```.`\n" "`      `:/++++++ooossssssssso++++syyyyyyyyyyyyyssyyyyyyyyyyyyyyysssoo+-``````-.\n" "`    ```/++ooooossssssyyyssso+++osyyyyyyyyhhhhyyyyyyyyyhhyyyyyyysssoo+:.`.-.---\n" "     ``./+ooooosssyyyyyyyyso++++syyyyyyyyyhhhhhhyyyyyhhhhhhhhyyyyssooo+-.+o//+/\n" "   ` ``-++ooosssyyyyyyyyysoo++oosyyhyyyyyyyhhhhhhyyyhhhhhhdhhhhyysssoo+:/ys+os/\n" "     `.:+oosssyyyyhhhhhyyso++osyyhhhhhyyyyyhhhhhyyyhhhhdddddhhhyyyssoo+/oyysyo-\n" "      ./+oossyyyhhhhhhhyso+ossyyhhdddhhhhhyyhhhhyyyhhhddddddhhhyyyssoo++shhys-`\n" "     ``-++oosyyyhhhhhhyso+osoossyhddhhhhhhhhhhhhyyyhhddddddhhhhyyyssoo+oyhho-``\n" "       `:++oosyyyhhhhyysoo++++++oyhyyyyysyyhhhhhhyyhhhdddhhhhyyyyyssoooshy+.```\n" "      ` ./++oossyyyyyyssooooooooosyyhdddhhhhhhhhhhyhhhhhhhhhyyyyyysooo++:.` ```\n" "      ```-+++oossssssssooooosssyyhhhhdddddhhhhhhhhyyyyhhhhhyyyyyyysoo+-`   ````\n" "      ```./++ooosssssoooossssyyyyyyhhhdddddddhhhhyyyysshhhhyyyyyysso+:`   `````\n" "        ``-+++oossssoo+/::/oossssyyyyhhhhhhhhhyo++syyssyhhyyyyyyysoo/.   ``.```\n" "          `:++ooosyssss/-`-oyydhhddddmmmmmmddhy::oyhhyyyhhyyyyyysso+-`   `..`.`\n" "      `  ` .:+oooossyssso+//+oyhdmmmmmmmddhsyyyhhhhhhhhhhhyyyyyssoo/.   ``..```\n" "     ` `````./++ooosssssssosssssyyhyyyyyyyyyyyhddhhhhhhhhyyyyyysoo+:`  ````````\n" "  `  ````````./++ooooossssssssssyyyyhhddhhhhdddddhhhhhhhyyyyyyssooo:` `````````\n" "    ``````````.:++++ooosssssssssyyyyyyhhhhdddddhhhhhhhhyyyyysssooo+:````....```\n" "   ````````````.://++ooosssssssyyyyyyyhhhdddddhhhhhhhhyyyyyysssoooo:`.......```\n" "  ```````````````.:/+++ooosssyyyyhhhhhhhdddddhhhhhhhyyyyyysssssssoo/-.........`\n" " `````````````````..:/+++oossyyyhhhhhhhdddddddhhhhyyyyyyssssssyssoo///:/-......\n" " ```````````````````..:/++oossyyyyhhhhhhhddhhhhhyyyyyyyysyyyyyyysso+:/ymo......\n" "``````````````````````.://+ooossyyyyyyyyhhhhhyyyyyyyyyyyyyyhyyyyssooymMNm/---..\n" "``````````````````````.:////++ooooosssssssssyyssssyyyyyyhhhhhhyysydNMMMNNh:--..\n" "```````````````````````-//////++++++++oooosssssyyyyyhhhhhhhhhyydNMMMMMMMMNy--..\n" "```````````````````````-//////+++ooooossssyyyyyyyhhhhhhhhhhhdNNMMMMMMMMMMMNo--.\n" "  ``````````````````.`.///+///+++oossssyyyyyyhhhhhhhhhhhdmNMMMMMMMMMMMMMMMMm:--\n" "  ````````````````...-/y//+++++++oosyyyyyyyhhhhhhhhhdmNMMMMMMMMMMMMMMMMMMMMMmyy\n" "  ``````````````.:/+soyds/+++++++oossyyyyhhhhhyhdmNNMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" "   ```````````-/ooossoydds/+++ooooossyyyyyyhdmNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" "   `````````-+oooooooohdddy//+++ooosssyydmNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" "  ``..-://++oooooooo++hmdmddy+//++oyhmNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNMMMMMMMM\n" ;
    int len = strlen(wot);
    for (int i = 0; i < 24; i++) {
        monitor_put('\n');
    }
    for (int i = 0; i < len; i++) {
        monitor_put(wot[i]);
        if (wot[i] == '\n') pause();
    }
    for (int i = 0; i < 24; i++) {
        monitor_put('\n');
        pause();
    }
    char *a[] = {"\n""\n""\n""\n""\n""\n""\n""\n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                            /$$\n""                                                                           |__ \n""                                                                              |\n""                                                                              |\n""                                                                         /$$  |\n""                                                                        | $$  |\n""                                                                        |  $$$$\n""                                                                         \\_____\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                    /$$$$$     \n""                                                                   |__  $$     \n""                                                                      | $$ /$$ \n""                                                                      | $$| $$ \n""                                                                 /$$  | $$| $$ \n""                                                                | $$  | $$| $$ \n""                                                                |  $$$$$$/|  $$\n""                                                                 \\______/  \\___\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                            /$$$$$             \n""                                                           |__  $$             \n""                                                              | $$ /$$   /$$  /\n""                                                              | $$| $$  | $$ |_\n""                                                         /$$  | $$| $$  | $$  /\n""                                                        | $$  | $$| $$  | $$ /$\n""                                                        |  $$$$$$/|  $$$$$$/|  \n""                                                         \\______/  \\______/  \\_\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                    /$$$$$                     \n""                                                   |__  $$                     \n""                                                      | $$ /$$   /$$  /$$$$$$  \n""                                                      | $$| $$  | $$ |____  $$|\n""                                                 /$$  | $$| $$  | $$  /$$$$$$$|\n""                                                | $$  | $$| $$  | $$ /$$__  $$|\n""                                                |  $$$$$$/|  $$$$$$/|  $$$$$$$|\n""                                                 \\______/  \\______/  \\_______/|\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                            /$$$$$                             \n""                                           |__  $$                             \n""                                              | $$ /$$   /$$  /$$$$$$  /$$$$$$$\n""                                              | $$| $$  | $$ |____  $$| $$__  $\n""                                         /$$  | $$| $$  | $$  /$$$$$$$| $$  \\ $\n""                                        | $$  | $$| $$  | $$ /$$__  $$| $$  | $\n""                                        |  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $\n""                                         \\______/  \\______/  \\_______/|__/  |__\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                    /$$$$$                                     \n""                                   |__  $$                                     \n""                                      | $$ /$$   /$$  /$$$$$$  /$$$$$$$   /$$$$\n""                                      | $$| $$  | $$ |____  $$| $$__  $$ /$$___\n""                                 /$$  | $$| $$  | $$  /$$$$$$$| $$  \\ $$| $$   \n""                                | $$  | $$| $$  | $$ /$$__  $$| $$  | $$| $$   \n""                                |  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$\n""                                 \\______/  \\______/  \\_______/|__/  |__/ \\_____\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                            /$$$$$                                         /$$ \n""                           |__  $$                                        | $$ \n""                              | $$ /$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$\n""                              | $$| $$  | $$ |____  $$| $$__  $$ /$$_____/| $$_\n""                         /$$  | $$| $$  | $$  /$$$$$$$| $$  \\ $$| $$      | $$ \n""                        | $$  | $$| $$  | $$ /$$__  $$| $$  | $$| $$      | $$ \n""                        |  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$ \n""                         \\______/  \\______/  \\_______/|__/  |__/ \\_______/|__/ \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                    /$$$$$                                         /$$         \n""                   |__  $$                                        | $$         \n""                      | $$ /$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$   /\n""                      | $$| $$  | $$ |____  $$| $$__  $$ /$$_____/| $$__  $$ /$\n""                 /$$  | $$| $$  | $$  /$$$$$$$| $$  \\ $$| $$      | $$  \\ $$| $\n""                | $$  | $$| $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$| $\n""                |  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$|  \n""                 \\______/  \\______/  \\_______/|__/  |__/ \\_______/|__/  |__/ \\_\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""            /$$$$$                                         /$$                 \n""           |__  $$                                        | $$                 \n""              | $$ /$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$ |\n""              | $$| $$  | $$ |____  $$| $$__  $$ /$$_____/| $$__  $$ /$$__  $$|\n""         /$$  | $$| $$  | $$  /$$$$$$$| $$  \\ $$| $$      | $$  \\ $$| $$  \\ $$|\n""        | $$  | $$| $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$| $$  | $$|\n""        |  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|\n""         \\______/  \\______/  \\_______/|__/  |__/ \\_______/|__/  |__/ \\______/  \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""    /$$$$$                                         /$$                  /$$$$$$\n""   |__  $$                                        | $$                 /$$__  $\n""      | $$ /$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$ | $$  \\ $\n""      | $$| $$  | $$ |____  $$| $$__  $$ /$$_____/| $$__  $$ /$$__  $$| $$  | $\n"" /$$  | $$| $$  | $$  /$$$$$$$| $$  \\ $$| $$      | $$  \\ $$| $$  \\ $$| $$  | $\n""| $$  | $$| $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$| $$  | $$| $$  | $\n""|  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$\n"" \\______/  \\______/  \\_______/|__/  |__/ \\_______/|__/  |__/ \\______/  \\______/\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""$$                                         /$$                  /$$$$$$   /$$$$\n""$$                                        | $$                 /$$__  $$ /$$__ \n""$$ /$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\\n""$$| $$  | $$ |____  $$| $$__  $$ /$$_____/| $$__  $$ /$$__  $$| $$  | $$|  $$$$\n""$$| $$  | $$  /$$$$$$$| $$  \\ $$| $$      | $$  \\ $$| $$  \\ $$| $$  | $$ \\____ \n""$$| $$  | $$ /$$__  $$| $$  | $$| $$      | $$  | $$| $$  | $$| $$  | $$ /$$  \\\n""$/|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/|  $$$$\n""/  \\______/  \\_______/|__/  |__/ \\_______/|__/  |__/ \\______/  \\______/  \\_____\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                   /$$                  /$$$$$$   /$$$$$$      \n""                                  | $$                 /$$__  $$ /$$__  $$     \n"" /$$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/     \n""| $$ |____  $$| $$__  $$ /$$_____/| $$__  $$ /$$__  $$| $$  | $$|  $$$$$$      \n""| $$  /$$$$$$$| $$  \\ $$| $$      | $$  \\ $$| $$  \\ $$| $$  | $$ \\____  $$     \n""| $$ /$$__  $$| $$  | $$| $$      | $$  | $$| $$  | $$| $$  | $$ /$$  \\ $$     \n""$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/|  $$$$$$/     \n""__/  \\_______/|__/  |__/ \\_______/|__/  |__/ \\______/  \\______/  \\______/      \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                           /$$                  /$$$$$$   /$$$$$$              \n""                          | $$                 /$$__  $$ /$$__  $$             \n""$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/       /$$   \n""__  $$| $$__  $$ /$$_____/| $$__  $$ /$$__  $$| $$  | $$|  $$$$$$       |  $$  \n""$$$$$$| $$  \\ $$| $$      | $$  \\ $$| $$  \\ $$| $$  | $$ \\____  $$       \\  $$/\n""__  $$| $$  | $$| $$      | $$  | $$| $$  | $$| $$  | $$ /$$  \\ $$        \\  $$\n""$$$$$$| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/|  $$$$$$/         \\  $\n""_____/|__/  |__/ \\_______/|__/  |__/ \\______/  \\______/  \\______/           \\_/\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                   /$$                  /$$$$$$   /$$$$$$                     /\n""                  | $$                 /$$__  $$ /$$__  $$                  /$$\n""$$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/       /$$    /$$|_  \n""$$__  $$ /$$_____/| $$__  $$ /$$__  $$| $$  | $$|  $$$$$$       |  $$  /$$/  | \n""$$  \\ $$| $$      | $$  \\ $$| $$  \\ $$| $$  | $$ \\____  $$       \\  $$/$$/   | \n""$$  | $$| $$      | $$  | $$| $$  | $$| $$  | $$ /$$  \\ $$        \\  $$$/    | \n""$$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/|  $$$$$$/         \\  $/    /$$\n""_/  |__/ \\_______/|__/  |__/ \\______/  \\______/  \\______/           \\_/    |___\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""           /$$                  /$$$$$$   /$$$$$$                     /$$      \n""          | $$                 /$$__  $$ /$$__  $$                  /$$$$      \n""  /$$$$$$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/       /$$    /$$|_  $$      \n"" /$$_____/| $$__  $$ /$$__  $$| $$  | $$|  $$$$$$       |  $$  /$$/  | $$      \n""| $$      | $$  \\ $$| $$  \\ $$| $$  | $$ \\____  $$       \\  $$/$$/   | $$      \n""| $$      | $$  | $$| $$  | $$| $$  | $$ /$$  \\ $$        \\  $$$/    | $$      \n""|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/|  $$$$$$/         \\  $/    /$$$$$$ /$$\n"" \\_______/|__/  |__/ \\______/  \\______/  \\______/           \\_/    |______/|__/\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""   /$$                  /$$$$$$   /$$$$$$                     /$$        /$$$$$\n""  | $$                 /$$__  $$ /$$__  $$                  /$$$$       /$$$_  \n""$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/       /$$    /$$|_  $$      | $$$$\\ \n""_/| $$__  $$ /$$__  $$| $$  | $$|  $$$$$$       |  $$  /$$/  | $$      | $$ $$ \n""  | $$  \\ $$| $$  \\ $$| $$  | $$ \\____  $$       \\  $$/$$/   | $$      | $$\\ $$\n""  | $$  | $$| $$  | $$| $$  | $$ /$$  \\ $$        \\  $$$/    | $$      | $$ \\ $\n""$$| $$  | $$|  $$$$$$/|  $$$$$$/|  $$$$$$/         \\  $/    /$$$$$$ /$$|  $$$$$\n""_/|__/  |__/ \\______/  \\______/  \\______/           \\_/    |______/|__/ \\______\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                /$$$$$$   /$$$$$$                     /$$        /$$$$$$      /\n""               /$$__  $$ /$$__  $$                  /$$$$       /$$$_  $$    /$\n""$$$   /$$$$$$ | $$  \\ $$| $$  \\__/       /$$    /$$|_  $$      | $$$$\\ $$   | $\n""  $$ /$$__  $$| $$  | $$|  $$$$$$       |  $$  /$$/  | $$      | $$ $$ $$   | $\n""\\ $$| $$  \\ $$| $$  | $$ \\____  $$       \\  $$/$$/   | $$      | $$\\ $$$$   | $\n""| $$| $$  | $$| $$  | $$ /$$  \\ $$        \\  $$$/    | $$      | $$ \\ $$$   | $\n""| $$|  $$$$$$/|  $$$$$$/|  $$$$$$/         \\  $/    /$$$$$$ /$$|  $$$$$$//$$|  \n""|__/ \\______/  \\______/  \\______/           \\_/    |______/|__/ \\______/|__/ \\_\n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""        /$$$$$$   /$$$$$$                     /$$        /$$$$$$      /$$$$$$  \n""       /$$__  $$ /$$__  $$                  /$$$$       /$$$_  $$    /$$$_  $$ \n""$$$$$ | $$  \\ $$| $$  \\__/       /$$    /$$|_  $$      | $$$$\\ $$   | $$$$\\ $$ \n""__  $$| $$  | $$|  $$$$$$       |  $$  /$$/  | $$      | $$ $$ $$   | $$ $$ $$ \n""  \\ $$| $$  | $$ \\____  $$       \\  $$/$$/   | $$      | $$\\ $$$$   | $$\\ $$$$ \n""  | $$| $$  | $$ /$$  \\ $$        \\  $$$/    | $$      | $$ \\ $$$   | $$ \\ $$$ \n""$$$$$/|  $$$$$$/|  $$$$$$/         \\  $/    /$$$$$$ /$$|  $$$$$$//$$|  $$$$$$/ \n""____/  \\______/  \\______/           \\_/    |______/|__/ \\______/|__/ \\______/  \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""/$$$$$$   /$$$$$$                     /$$        /$$$$$$      /$$$$$$          \n""$$__  $$ /$$__  $$                  /$$$$       /$$$_  $$    /$$$_  $$         \n""$$  \\ $$| $$  \\__/       /$$    /$$|_  $$      | $$$$\\ $$   | $$$$\\ $$         \n""$$  | $$|  $$$$$$       |  $$  /$$/  | $$      | $$ $$ $$   | $$ $$ $$         \n""$$  | $$ \\____  $$       \\  $$/$$/   | $$      | $$\\ $$$$   | $$\\ $$$$         \n""$$  | $$ /$$  \\ $$        \\  $$$/    | $$      | $$ \\ $$$   | $$ \\ $$$         \n"" $$$$$$/|  $$$$$$/         \\  $/    /$$$$$$ /$$|  $$$$$$//$$|  $$$$$$/         \n""______/  \\______/           \\_/    |______/|__/ \\______/|__/ \\______/          \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""  /$$$$$$                     /$$        /$$$$$$      /$$$$$$                  \n"" /$$__  $$                  /$$$$       /$$$_  $$    /$$$_  $$                 \n""| $$  \\__/       /$$    /$$|_  $$      | $$$$\\ $$   | $$$$\\ $$                 \n""|  $$$$$$       |  $$  /$$/  | $$      | $$ $$ $$   | $$ $$ $$                 \n"" \\____  $$       \\  $$/$$/   | $$      | $$\\ $$$$   | $$\\ $$$$                 \n"" /$$  \\ $$        \\  $$$/    | $$      | $$ \\ $$$   | $$ \\ $$$                 \n""|  $$$$$$/         \\  $/    /$$$$$$ /$$|  $$$$$$//$$|  $$$$$$/                 \n"" \\______/           \\_/    |______/|__/ \\______/|__/ \\______/                  \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""$                     /$$        /$$$$$$      /$$$$$$                          \n""$$                  /$$$$       /$$$_  $$    /$$$_  $$                         \n""_/       /$$    /$$|_  $$      | $$$$\\ $$   | $$$$\\ $$                         \n""$       |  $$  /$$/  | $$      | $$ $$ $$   | $$ $$ $$                         \n""$$       \\  $$/$$/   | $$      | $$\\ $$$$   | $$\\ $$$$                         \n""$$        \\  $$$/    | $$      | $$ \\ $$$   | $$ \\ $$$                         \n""$/         \\  $/    /$$$$$$ /$$|  $$$$$$//$$|  $$$$$$/                         \n""/           \\_/    |______/|__/ \\______/|__/ \\______/                          \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""              /$$        /$$$$$$      /$$$$$$                                  \n""            /$$$$       /$$$_  $$    /$$$_  $$                                 \n"" /$$    /$$|_  $$      | $$$$\\ $$   | $$$$\\ $$                                 \n""|  $$  /$$/  | $$      | $$ $$ $$   | $$ $$ $$                                 \n"" \\  $$/$$/   | $$      | $$\\ $$$$   | $$\\ $$$$                                 \n""  \\  $$$/    | $$      | $$ \\ $$$   | $$ \\ $$$                                 \n""   \\  $/    /$$$$$$ /$$|  $$$$$$//$$|  $$$$$$/                                 \n""    \\_/    |______/|__/ \\______/|__/ \\______/                                  \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""      /$$        /$$$$$$      /$$$$$$                                          \n""    /$$$$       /$$$_  $$    /$$$_  $$                                         \n""/$$|_  $$      | $$$$\\ $$   | $$$$\\ $$                                         \n""$$/  | $$      | $$ $$ $$   | $$ $$ $$                                         \n""$/   | $$      | $$\\ $$$$   | $$\\ $$$$                                         \n""/    | $$      | $$ \\ $$$   | $$ \\ $$$                                         \n""    /$$$$$$ /$$|  $$$$$$//$$|  $$$$$$/                                         \n""   |______/|__/ \\______/|__/ \\______/                                          \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""$        /$$$$$$      /$$$$$$                                                  \n""$       /$$$_  $$    /$$$_  $$                                                 \n""$      | $$$$\\ $$   | $$$$\\ $$                                                 \n""$      | $$ $$ $$   | $$ $$ $$                                                 \n""$      | $$\\ $$$$   | $$\\ $$$$                                                 \n""$      | $$ \\ $$$   | $$ \\ $$$                                                 \n""$$$ /$$|  $$$$$$//$$|  $$$$$$/                                                 \n""__/|__/ \\______/|__/ \\______/                                                  \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n"" /$$$$$$      /$$$$$$                                                          \n""/$$$_  $$    /$$$_  $$                                                         \n"" $$$$\\ $$   | $$$$\\ $$                                                         \n"" $$ $$ $$   | $$ $$ $$                                                         \n"" $$\\ $$$$   | $$\\ $$$$                                                         \n"" $$ \\ $$$   | $$ \\ $$$                                                         \n""  $$$$$$//$$|  $$$$$$/                                                         \n""\\______/|__/ \\______/                                                          \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""      /$$$$$$                                                                  \n""$    /$$$_  $$                                                                 \n""$   | $$$$\\ $$                                                                 \n""$   | $$ $$ $$                                                                 \n""$   | $$\\ $$$$                                                                 \n""$   | $$ \\ $$$                                                                 \n""//$$|  $$$$$$/                                                                 \n""|__/ \\______/                                                                  \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""$$$$$                                                                          \n""$_  $$                                                                         \n""$$\\ $$                                                                         \n"" $$ $$                                                                         \n""\\ $$$$                                                                         \n"" \\ $$$                                                                         \n""$$$$$/                                                                         \n""____/                                                                          \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                             \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""\n""\n""\n""\n""\n""\n""\n""\n","\n""\n""\n""\n""\n""\n""\n""\n""                                                                     \n""                                                                          \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                               \n""                                                                              \n""\n""\n""\n""\n""\n""\n""\n""\n"}

        ;
    for (int i = 0; i < 29; i++) {
        monitor_write(a[i]);
        for (int j = 0; j < 10; j++) {
            pause();
        }
    }
    monitor_clear();
}

void real_intro() {
    monitor_clear();
    int nospoilers[] = {83, 121, 115, 116, 101, 109, 32, 102, 105, 110, 105, 115, 104, 101, 100, 32, 98, 111, 111, 116, 105, 110, 103, 46, 10, 87, 101, 108, 99, 111, 109, 101, 32, 116, 111, 32, 74, 117, 97, 110, 99, 104, 111, 79, 83, 32, 118, 49, 46, 48, 46, 48, 10, 10};
    for (int i = 0; i < 54; i++) monitor_put((char) nospoilers[i]);
}
