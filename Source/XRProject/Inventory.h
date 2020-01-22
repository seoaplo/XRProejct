#pragma once

const int kInventoryHeight = 6;
const int kInventoryWidth = 5;
const int kMaxInventorySlotSize = kInventoryHeight * kInventoryWidth;

#include "CustomSingleton.h"
#include "Item.h"

class Inventory : public CustomSingleton<Inventory>
{
public:
	friend class CustomSingleton<Inventory>;
private:
	Inventory();
public:
	~Inventory();
private:
	UItem* Slot[kMaxInventorySlotSize];
public:
	int GetInventoryHeight();
	int GetInventoryWidth();
	int GetInventorySize();
	UItem* GetItem(int index);
	bool AddItem(UItem* item, int index = -1); // -1�� ��� �� ���� ã�Ƽ� �ֱ�, �� ���� ��� �ش� �ε�����
	UItem* ExchangeItem(UItem* item, int index); // �ش� �ε����� ���Կ� �������� �ְ� �޾ư� ������ ��������
};