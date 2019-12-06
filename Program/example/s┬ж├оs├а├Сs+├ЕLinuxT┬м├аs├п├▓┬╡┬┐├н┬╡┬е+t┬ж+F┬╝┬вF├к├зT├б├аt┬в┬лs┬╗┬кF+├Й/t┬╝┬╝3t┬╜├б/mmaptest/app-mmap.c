/*�������ܣ�ʵ��4��LED�Ƶ�ͬʱ���𣬼��Ϊ1��*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DEV_NAME "/dev/mmapled"

int main() {
    int fd, k;
    void* start, *reg = NULL;

    /* fd = open(DEV_NAME,O_RDWR);

    */

    if ((fd = open("/dev/mem", O_RDWR)) < 0) {
        printf("Open mem dev failed!\n");
        return -1;
    }


    if (fd < 0) {
        printf("Open device err!\n");
        return -1;
    }

    /*��������:
     * NULL:ӳ�䵽���ں������ַ,����ΪNULL���ں˾���
     * 1024*4:ӳ���С,��Сһҳ,��Ϊҳ��С��������
     * ӳ������Ȩ��
     * ��ӳ�������޸ĸı�ӳ����������
     * fd:open���ص��ļ�������
     * �����ַ,һ��ҳ����ʼ�����ַ,��>>PAGE_SHIFT֮�󴫸�������vma�ṹ���vm_pgoff
     */
    /*0x56000000��LED�����ڵ�GPIO�ڵ�BANK��ʼ�����ַ*/
    /*start�ǵĵõ��������ַ*/
    start = mmap(NULL, 1024 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x56000000);

    if (start == NULL) {
        printf("mmap err!\n");
        return -1;
    }

    reg = start + 0x10;            //GPBCON�����ƼĴ���
    *(unsigned*)reg &= 0xfffc03ff; //[17:10]����
    *(unsigned*)reg |= 0x00015400; //[17:10]=01010101,�������
    reg = start + 0x14;            //GPBDAT

    /*����k�Σ�ʵ�ֶ�LED�Ĳ���*/
    k = 25;

    while (k--) {
        *(unsigned*)reg &= ~(0x1e0); //[8:5],set 0,led on
        sleep(1);
        *(unsigned*)reg |= 0x1e0;    //[8:5],set 1,led off
        sleep(1);
        printf("k= %d\n", k);
    }

    /*ȡ��ӳ��*/
    munmap(start, 1024 * 4);
    close(fd);
    return 0;
}