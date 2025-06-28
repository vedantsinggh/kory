#include "cli.h"

int main(int argc, char* argv[]){
    cli app;
    Output out = app.parse_args(argc,argv);
    app.run(out);
    return 0;
}
