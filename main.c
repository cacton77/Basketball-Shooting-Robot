/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "math.h"

#define STOP 0
#define CW 1
#define CCW 2
#define FOUND 1
#define LOST 0

/*                  *\
   Global Variables
\*                  */

char state = 'X';

//LIDAR
uint8 cb = 0;
uint8 lindex = 0;
uint8 bytes[9];
uint16 cm = 0;
uint16 signal = 0;
uint16 lastcm = 0;
uint32 to = 0;

//Compare cm
uint8 compareCm = 0;
uint8 objectdet = LOST;

uint16 cmavg = 0;
uint16 cmavglast = 0;

//Stepper 1
uint8 dir = STOP;
uint8 break_counter = 5;
uint8 step_counter = 0;
uint8 step_index = 0;

char datapacket[256];

//Testing Variables REMOVE LATER
uint8 blinky = 0;

CY_ISR(isr_1)
{
    /*  OLD INTERRUPT
    sprintf(datapacket, "%c, cm: %d, signal: %d\r\n", state, cm, signal);
    //sprintf(datapacket, "ISR1 Time: %lu\r\n", to);
    UART_COM_PutString(datapacket);
    if (UART_COM_GetRxBufferSize()!=0)
    {
        state = UART_COM_GetChar();
    }
    to = Timer_1_ReadCapture();
    */
    Control_Reg_Write(0);
    Status_Reg_Read();
}

CY_ISR(isr_2)
{
    blinky = 0;
    if (dir == STOP)
    {
        A_1_Write(0);
        B_1_Write(0);
        C_1_Write(0);
        D_1_Write(0);
        step_index = 0;
        step_counter = 0;
        break_counter = 0;
    }
    else if (dir == CW)
    {
        if (break_counter > 0)
        {
            break_counter--;
        }
        else
        {
            if (step_index == 0)
            {
                A_1_Write(1);
                B_1_Write(0);
                C_1_Write(1);
                D_1_Write(0);
                step_index++;
            }
            else if (step_index == 1)
            {
                A_1_Write(1);
                B_1_Write(0);
                C_1_Write(0);
                D_1_Write(1);
                step_index++;
            }
            else if (step_index == 2)
            {
                A_1_Write(0);
                B_1_Write(1);
                C_1_Write(0);
                D_1_Write(1);
                step_index++;
            }
            else
            {
                A_1_Write(0);
                B_1_Write(1);
                C_1_Write(1);
                D_1_Write(0);
                step_index = 0;
            }
            if (step_counter < 255)
            {
                step_counter++;
            }
            //break_counter = 5 - step_counter;
        }
    }
    else if (dir == CCW)
    {
        if (break_counter > 0)
        {
            break_counter--;
        }
        else
        {
            if (step_index == 0)
            {
                A_1_Write(1);
                B_1_Write(0);
                C_1_Write(0);
                D_1_Write(1);
                step_index++;
            }
            else if (step_index == 1)
            {
                A_1_Write(1);
                B_1_Write(0);
                C_1_Write(1);
                D_1_Write(0);
                step_index++;
            }
            else if (step_index == 2)
            {
                A_1_Write(0);
                B_1_Write(1);
                C_1_Write(1);
                D_1_Write(0);
                step_index++;
            }
            else
            {
                A_1_Write(0);
                B_1_Write(1);
                C_1_Write(0);
                D_1_Write(1);
                step_index = 0;
            }
            if (step_counter < 255)
            {
                step_counter++;
            }
            //break_counter = 5 - step_counter;
        }
    }
    else
    {
        state = 'E';
    }
    Control_Reg_Write(1);
    Status_Reg_Read();
}

void GetLidarData(void)
{   
    for(;;)
    {
        uint8 cb = UART_LIDAR_GetRxBufferSize();
        if (cb>0)
        {
            bytes[lindex] = UART_LIDAR_ReadRxData();
            if (lindex == 0 && bytes[0] == 0x59)
            {
                lindex++;
                continue;
            }
            if (lindex == 1 && bytes[1] != 0x59)
            {
                lindex = 0;
                continue;
            }
            if (lindex >= 1 && lindex < 9)
            {
                lindex++;
                continue;
            }
            if (lindex == 9)
            {
                uint8 checksum = 0;
                for(int i = 0; i < 8; i++)
                {
                    checksum += bytes[i];
                }
                if(checksum == bytes[8])
                {
                    lindex = 0;
                    cm = (bytes[3] << 8) + bytes[2];
                    signal = (bytes[5] << 8) + bytes[4];
                    //cmsum += cm;
                    //cmnum++;
                    //sigsum += signal;
                    //signum++;
                    return;
                }
                else
                {
                    lindex = 0;
                    continue;
                }
                lindex = 0;
            }
        }
    }
}

int main(void)
{
    isr_1_Start();
    isr_1_StartEx(isr_1);
    
    isr_2_Start();
    isr_2_StartEx(isr_2);
    
    Timer_1_Start();
    
    UART_COM_Start();
    UART_LIDAR_Start();
    
    Status_Reg_InterruptEnable();
    
    CyGlobalIntEnable;

    for(;;)
    {
        blinky++;
        lastcm = cm;
        GetLidarData();
        if (cm > lastcm+100)
        {
            objectdet = LOST;
        }
        else if (cm < lastcm-100)
        {
            objectdet = FOUND;
        }
        sprintf(datapacket, "%c, cm: %d, Blink: %d, Object: %d\r\n", state, cm, blinky, objectdet);
        //sprintf(datapacket, "ISR1 Time: %lu\r\n", to);
        UART_COM_PutString(datapacket);
        if (UART_COM_GetRxBufferSize()!=0)
        {
            state = UART_COM_GetChar();
        }
        if (state == 'S' || state == 's')
        {
            dir = CW;
        }
        else if (state == 'B' || state == 'b')
        {
            dir = CCW;
        }
        else if (state == 'X' || state == 'x')
        {
            dir = STOP;
        }
    }
}
