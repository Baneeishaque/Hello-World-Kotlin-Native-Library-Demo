#include "native_api.h"
#include "stdio.h"

int main(int argc, char** argv) {
    //obtain reference for calling Kotlin/Native functions
    native_ExportedSymbols* lib = native_symbols();

    lib->kotlin.root.example.forIntegers(1, 2, 3, 4);
    lib->kotlin.root.example.forFloats(1.0f, 2.0);

    //use C and Kotlin/Native strings
    const char* str = "Hello from Native!";
    const char* response = lib->kotlin.root.example.strings(str);
    printf("in: %s\nout:%s\n", str, response);
    lib->DisposeString(response);

    //create Kotlin object instance
    native_kref_example_Clazz newInstance = lib->kotlin.root.example.Clazz.Clazz();
    int x = lib->kotlin.root.example.Clazz.memberFunction(newInstance, 42);
    lib->DisposeStablePointer(newInstance.pinned);

    printf("DemoClazz returned %d\n", x);

    return 0;
}
