#ifndef HASHSET_H
#define HASHSET_H
#include  "hashtable.h"
//自定义子类.

class HashSet : public HashTable
{
public:
    HashSet(int nLen) :HashTable(nLen){}
    ~HashSet(){ }
    //friend void hashInterSection(HashSet<int>* pHashSet, int a[], int b[], int m, int n);
public:
     void hashInterSection(HashSet *pHashSet, QList<int>a, QList<int>b, int m, int n,QList<int>&linePoint)
     {
         for (int i = 0; i < m; i++)
         {
             pHashSet->InsertHash(a[i],linePoint);
         }

         for (int j = 0; j < n; j++)
         {
             pHashSet->InsertHash(b[j],linePoint);
         }
     }

};
#endif // HASHSET_H
