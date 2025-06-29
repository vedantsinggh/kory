#include "cli.h"
#include "backup_engine.h"

int main(int argc, char* argv[]){
    cli app;
    Output out = app.parse_args(argc,argv);

    BackupEngine engine;
    engine.log(HALT, "this should halt!");

    app.run(out);
    return 0;
}
