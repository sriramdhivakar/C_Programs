#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/ioctl.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <sys/select.h>

#include <sys/time.h>

#include <errno.h>

int main(void)

{

    int buttons_fd;

    char buttons[6] = {'0', '0', '0', '0', '0', '0'}; //���尴��ֵ�������������������е�key_values ����

    /*�򿪰����豸/dev/buttons*/

    buttons_fd = open("/dev/inters", 0);

    if (buttons_fd < 0)

    {

        /*��ʧ�����˳�*/

        perror("open device buttons");

        exit(1);

    }

    /*������������ӡ��ֵ��״̬*/

    for (;;)

    {

        char current_buttons[6];

        int count_of_changed_key;

        int i;

        /*ʹ��read ������ȡһ�鰴��ֵ��6 ����*/

        if (read(buttons_fd, current_buttons, sizeof current_buttons) != sizeof current_buttons)

        {

            perror("read buttons:");

            exit(1);

        }

        /*���������ȡ���İ���ֵ*/

        for (i = 0, count_of_changed_key = 0; i < sizeof buttons / sizeof buttons[0]; i++) {

            if (buttons[i] != current_buttons[i])

            {

                buttons[i] = current_buttons[i];

                /*��ӡ����ֵ����������������/̧���״̬*/

                printf("%skey %d is %s", count_of_changed_key ? ", " : "", i + 1, buttons[i] == '0' ? "up" :

                       "down");

                count_of_changed_key++;

            }

        }

        if (count_of_changed_key)

        {

            printf("\n");

        }

    }

    /*�رհ����豸�ļ�*/

    close(buttons_fd);

    return 0;

}
