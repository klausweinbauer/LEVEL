#include <CAM.h>
#include <iostream>

int main(int argc, char** argv) {
    CAM_t* cam = new CAM_t();
    cam->cam.camParameters.lowFrequencyContainer = new LowFrequencyContainer();

    std::cout << "Hello World" << std::endl;

    /*FILE* file;
    file = fopen("cam.log", "w+");
    asn_fprint(file, &asn_DEF_CAM, cam);
    fseek(file, 0, SEEK_SET);
    int c;
    while ((c = getc(file)) != EOF) {
        putchar(c);
    }
    delete file;*/
    
    ASN_STRUCT_FREE(asn_DEF_CAM, cam);
}