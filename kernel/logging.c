#include <klib/klib.h>

#include "arch/arch.h"
#include "arch/serial.h"

static char *mod_name;
static char *const witty_comments[] = {
    "Witty comment unavailable :(",

    "Excuse me Sir, \n\t"
    "// Do you have a moment to talk about TempleOS?",

    "Surprise! Haha. Well, this is awkward.",
    "Oh - I know what I did wrong!",
    "Uh... Did I do that?",
    "Oops.",
    "On the bright side, I bought you a teddy bear!",
    "DON'T PANIC!",
    "...",
    "Greenpeace free'd the mallocs \\o/",
    "Typo in the code.",
    "System consumed all the paper for paging!",
    "Suspicious pointer corrupted the machine.",
    "I'm tired of this ;_;",
    "PC LOAD LETTER",
    "Abort, Retry, Fail?",
    "Bad command or file name",
    "OOF!",
    "OoooOOoOoOF!",
    "Et là c'est le drame...",
    "Everything's going to plan. No, really, that was supposed to happen.",
    "My bad.",
    "Minecraft crashed!",
    "Quite honestly, I wouldn't worry myself about that.",
    "This doesn't make any sense!",
    "It's not a good surprise...",
    "Don't do that.",
    "Get the f*** outa my room, I'm playing minecraft",
    "Fuck I needed that ;"
};

static char *get_witty_comment() {
	return witty_comments[arch_rdtsc() % ARRAY_LENGTH(witty_comments)];
}

void logging_debug(char *msg) {
	serial_write_string(mod_name);
        serial_write_string(": ");
        serial_write_string("debug: ");
        serial_write_string(msg);
        serial_write_string("\n");
}

void logging_log(char *msg) {
        serial_write_string(mod_name);
        serial_write_string(": ");
        serial_write_string(msg);
        serial_write_string("\n");
}

void logging_panic(char *msg) {
        serial_write_string("\nPANIC: ");
	serial_write_string(mod_name);
        serial_write_string(": ");
        serial_write_string(msg);
        serial_write_string("\n// ");
	serial_write_string(get_witty_comment());
	serial_write_string("\n");

	arch_halt();
}

void logging_module(char *name) {
	mod_name = name;
}
