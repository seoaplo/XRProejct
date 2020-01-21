#pragma once

const int kMaxInventorySlotSize = 30;

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
	bool AddItem(UItem* item, int index = -1); // -1�� ��� �� ���� ã�Ƽ� �ֱ�, �� ���� ��� �ش� �ε�����
	UItem* ExchangeItem(UItem* item, int index); // �ش� �ε����� ���Կ� �������� �ְ� �޾ư� ������ ��������
};