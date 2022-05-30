#include <stdio.h>
#include <stdlib.h>

#include "int_stack.h"

/*
 * Minimum working example for int_stack.c.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   2022-03-24: v1.0, first public version.
 */

int main(void)
{
	// Create the stack.
	stack s=stack_empty();

	for (int i=1; i<=3; i++) {
	    // Push value on stack.
	    s = stack_push(s, i);
	}

	printf("--STACK before popping--\n");
        stack_print(s);

        // Pop element from stack.
        s=stack_pop(s);

        printf("--STACK after popping--\n");
        stack_print(s);

	// Kill stack. Strictly not necessary for int_stack since it
	// does not use any dynamic memory. Included for symmetry with
	// stack.c.
	stack_kill(s);

	return 0;
}
