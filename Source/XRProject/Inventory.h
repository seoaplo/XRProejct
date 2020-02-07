#pragma once
#include "CustomSingleton.h"
#include "Item.h"

const int kInventoryHeight = 6;
const int kInventoryWidth = 5;
const int kMaxInventorySlotSize = kInventoryHeight * kInventoryWidth;

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
	bool UseSlot(int SlotID);
	int64 GetGold();
	void SetGold(int64 Money);
	void AddGold(int64 Money);
	int GetInventoryHeight();
	int GetInventoryWidth();
	int GetInventorySize();
	int GetItemCount(int64 ID);
	UItem* GetItem(int index); // ������ ��ü ����
	void SetItem(UItem* Item, int Index);
	bool AddItem(UItem* item, int index); // -1�� ��� �� ���� ã�Ƽ� �ֱ�, �� ���� ��� �ش� �ε�����
	void ExchangeItem(int index1, int index2);
};