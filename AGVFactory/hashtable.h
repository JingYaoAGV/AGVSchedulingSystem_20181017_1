#ifndef HASHTABLE_H
#define HASHTABLE_H
//template <typename _Type>
class HashTable
{
public:
    HashTable(int Length)   //构建哈希表，表长Length
    {
        Element = new int[Length];
        for (int i = 0; i<Length; i++)
        {
            Element[i] = -1;
        }
        this->Length = Length;
        Count = 0;
    }

    ~HashTable()
    {
        delete[] Element;
        //qDebug()<<"aaa";
    }

    //求哈希地址
    virtual int Hash(int Data)
    {
        return Data % Length; //³ýÁôÓàÊý·¨Çó¹þÏ£µØÖ·.
    }

    //开放定址法再哈希
    virtual int ReHash(int Index, int Count)
    {
        return ((Index + Count) % Length); //
    }

    //查找元素，若已存在返回true，否则返回false。
    virtual bool SerachHash(int Data, int& Index)
    {
        Index = Hash(Data);
        int Count = 0;

        while (Element[Index] != -1 && Element[Index] != Data)
        {
            Index = ReHash(Index, ++Count);
        }
        return (Data == Element[Index] ? true : false);
    }

    virtual int SerachHash(int Data)
    {
        int Index = 0;
        if (SerachHash(Data, Index))
        {
            return Index;
        }
        else
        {
            return -1;
        }
    }

    // 插入元素
    bool InsertHash(int Data,QList<int>&points)
    {
        int Index = 0;
        if (Count < Length && !SerachHash(Data, Index))
        {
            Element[Index] = Data;
            Count++;
            return true;
        }
        //在插入的过程中，如果元素已经存在，即为交集元素则打印之.
        if (SerachHash(Data, Index))
        {
           // cout << Data << "\t";
            //qDebug()<<Data;
            points.append(Data);
        }
        return false;
    }









protected:
    int* Element;           // Hash表
    int Length;               // Hash表长度
    int Count;                // Hash表当前长度

};

#endif // HASHTABLE_H
