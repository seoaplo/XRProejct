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
	int64 Gold;
public:
	int64 GetGold();
	void SetGold(int64 Money);
	void AddGold(int64 Money);
	int GetInventoryHeight();
	int GetInventoryWidth();
	int GetInventorySize();
	int GetItemCount(int64 ID);
	UItem* GetItem(int index); // ������ ��ü ����
	bool AddItem(UItem* item, int index = -1); // -1�� ��� �� ���� ã�Ƽ� �ֱ�, �� ���� ��� �ش� �ε�����
	bool ExchangeItem(int index1, int index2); // �ش� �ε����� ���Կ� �������� �ְ� �޾ư� ������ ��������
	bool UseSlot(int SlotId);
};