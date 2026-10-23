#include <stdio.h>
#include <windows.h>

#define N 12

// ВОТ АНКЕТА без дописи

typedef struct {
    int minSnr, maxSnr;              // min и max SNR
    int minIndex, maxIndex;         // min и max SNR
    int sumSnr;                    // сумма для подсчёта среднего
    int countGood, countOk;       // счётчики клиентов >= 35 дБ и >= 20 дБ
    float avgSnr;                // среднее SNR
} SnrReport;

// функция для оценивания качества сигнала - бумажка с адресом
const char* snrQuality(int snr)
{
    if (snr >= 35)
    {
        return "Отличный";
    }
    else if (snr >= 20)
    {
        return "Хороший";
    }
    else if (snr >= 0)
    {
        return "Приемлемый";
    }
    else
    {
        return "Плохой";
    }
}

void printReport(SnrReport report, int n)
{
    printf("Отчёт по СОТЕ Wi-Fi (N = %d)\n\n", n);
    
    printf("Минимальный SNR: %d дБ (клиент %02d) - %s\n", report.minSnr, report.minIndex, snrQuality(report.minSnr));
    printf("Максимальный SNR: %d дБ (клиент %02d) - %s\n", report.maxSnr, report.maxIndex, snrQuality(report.maxSnr));
    printf("Средний SNR : %.2f дБ\n\n", report.avgSnr);
     
    int countBad = n - report.countOk; 
    
    printf("Клиентов с SNR >= 35 дБ : %d\n", report.countGood);
    printf("Клиентов с SNR >= 20 дБ : %d\n", report.countOk); 
    printf("Клиентов с SNR < 20 дБ : %d <- требуют внимания\n\n", countBad);
    
    printf("Разброс (max-min) : %d дБ\n\n", report.maxSnr - report.minSnr);

 
    if (report.countOk == n)
    {
        printf("Оценка соты: отличная\n");
    }
    else if (report.countOk >= n / 2)
    {
        printf("Оценка соты: рабочая, но есть проблемные клиенты\n");
    }
    else
    {
        printf("Оценка соты: плохая, много проблемных клиентов\n");
    }
}

int main()
{   
    SetConsoleOutputCP(65001);

    // массив SNR 
    int snr[N] = { -42, -38, -25, -17, -33, -45, -29, -12, -36, -41, -22, 30 };

    int i; 
    
    // Создаем нашу коробку
    SnrReport report;
    report.minSnr = snr[0];
    report.maxSnr = snr[0];
    report.minIndex = 0;
    report.maxIndex = 0;
    report.sumSnr = 0;
    report.countGood = 0;
    report.countOk = 0;

    // ищем min, max, сумму и считаем клиентов по порогам
    for (i = 0; i < N; i++)
    {
        if (snr[i] < report.minSnr)        // поиск минимум минимального
        {
            report.minSnr = snr[i];
            report.minIndex = i;           // id клиента 
        }
        if (snr[i] > report.maxSnr)        // поиск максимум максимального
        {
            report.maxSnr = snr[i];
            report.maxIndex = i;
        }
        report.sumSnr = report.sumSnr + snr[i];   // саму себя + итерация цикла

        if (snr[i] >= 35)           // good канал
        {
            report.countGood = report.countGood + 1;
        }
        if (snr[i] >= 20)           // norm канал
        {
            report.countOk = report.countOk + 1;
        }
    }

    // считаем среднее, пушка не нужна
    report.avgSnr = (float)report.sumSnr / N;

    // вызываем функцию вывода и передаем ей нашу коробку
    printReport(report, N);

    return 0;
}

    // конец
