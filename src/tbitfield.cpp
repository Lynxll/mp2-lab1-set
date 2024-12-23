// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <iostream>
#include <algorithm> // для std::max и std::min

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) : BitLen(len) //len - длина битового поля
{
    if (len < 0)
    {
        throw std::exception();
    }
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen]; // выделение памяти
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0; // обнуление памяти
    }
}
// конструктор копирования
TBitField::TBitField(const TBitField& bf) : BitLen(bf.BitLen), MemLen(bf.MemLen)
{
    pMem = new TELEM[MemLen];//выделение памяти для массива
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i]; // копирование данных
    }
}

TBitField::~TBitField() { // деструктор
    delete[] pMem; // освобождение памяти
}

int TBitField::GetMemIndex(const int n) const { // индекс Мем для бита n
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const { // битовая маска для бита n
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const { // получить длину (к-во битов)
    return BitLen;
}

void TBitField::SetBit(const int n) {// установить бит
    if (n < 0 || n>BitLen)
    {
        throw std::exception();
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);

}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n>BitLen)
    {
        throw std::exception();
    }

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);

}

int TBitField::GetBit(const int n) const {
    if (n < 0 || n>BitLen)
    {
        throw std::exception();
    }// получить значение бита

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;

    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) { // присваивание
    if (this != &bf) { // проверка на самоприсваивание
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const { // сравнение
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const { // сравнение
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) { // операция "или"
    int len = std::max(BitLen, bf.BitLen);
    TBitField res(len);
    int minLen = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < minLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) { // операция "и"
    int len = std::max(BitLen, bf.BitLen);
    TBitField res(len);
    int minLen = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < minLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) { // отрицание
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }

    // Маска для обнуления лишних битов
    if (BitLen % (sizeof(TELEM) * 8) != 0) {
        TELEM mask = (static_cast<TELEM>(1) << (BitLen % (sizeof(TELEM) * 8))) - 1;
        res.pMem[MemLen - 1] &= mask;
    }

    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) { // ввод
    for (int i = 0; i < bf.MemLen; i++) {
        istr >> bf.pMem[i];
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) { // вывод
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
