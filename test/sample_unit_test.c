#include <criterion/criterion.h>
#include <stdio.h>
#include "cli.h"

void setup(void) {
    puts("Runs before the test");
}

void teardown(void) {
    puts("Runs after the test");
}

Test(simple, fixtures, .init = setup, .fini = teardown) {
    CLIArguments* c = cli_config_default();
    cr_assert(c != NULL);
    cli_config_destroy(c);
    c = NULL;
    cr_assert(c == NULL);
}
