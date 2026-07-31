#include <stdlib.h>
#include "base_layer/base.c"
#include "base_layer/base_memory.c"

typedef struct{
    char a;
    char b;
    char c;
    char d;
} Person;

int main(){
    Arena arena1;
    Arena arena2;
    arena_init(&arena1, sizeof(Person));
    arena_init(&arena2, sizeof(Person));
    Person person = {};
    person.a = 'a';
    person.b = 'b';
    person.c = 'c';
    person.d = 'd';
    arena_push(&arena1, &person, sizeof(Person));
    arena_push(&arena2, &person, sizeof(Person));
    arena_clear_zeroed(&arena1);
}