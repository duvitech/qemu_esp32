#include <stdio.h>
//
// qemu_toflash
// 
// python /home/olas/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 
// 0x1000  /home/olas/esp/esp-idf/examples/system/ota/build/bootloader/bootloader.bin  
// 0x10000 /home/olas/esp/esp-idf/examples/system/ota/build/ota.bin 
// 0x8000 /home/olas/esp/esp-idf/examples/system/ota/build/partitions_two_ota.bin
//

void merge_flash(char *binfile,char *flashfile,int flash_pos)
{
    FILE *fbin;
    FILE *fflash;
    char *tmp_data;

    int file_size=0;

    fbin = fopen(binfile, "r");
    if (fbin == NULL) {
        printf("   Can't open '%s' for reading.\n", binfile);
		return;
	}

    if (fseek(fbin, 0 , SEEK_END) != 0) {
       /* Handle Error */
    }
    file_size = ftell(fbin);
    if (fseek(fbin, 0 , SEEK_SET) != 0) {
      /* Handle Error */
    }

    fflash  = fopen(flashfile, "w");
    if (fflash == NULL) {
        printf("   Can't open '%s' for writing.\n", flashfile);
        return;
    }
    fseek(flashfile,flash_pos,SEEK_SET);

    tmp_data=malloc(file_size*sizeof(char));
    //size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    //size_t fwrite(const void *ptr, size_t size, size_t nmemb,
    //                 FILE *stream);


    fread(tmp_data,file_size,sizeof(char),fbin);

    fwrite(tmp_data,file_size,sizeof(char),flashfile);

    fclose(fbin);

    fclose(flashfile);

}


int main(int argc,char *argv[])
{
    // system("cp esp32flash.bin esp32flash.bak");

    // Add bootloader
    merge_flash("bootloader/bootloader.bin","esp32flash.bin",0x1000);
    // Add partitions, test OTA here
    merge_flash("build/partitions_two_ota.bin","esp32flash.bin",0x8000);
    // Add application
    merge_flash(argv[1],"esp32flash.bin",0x10000);

}
