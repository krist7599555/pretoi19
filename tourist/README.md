# Tourist นักท่องเที่ยว

[🎉 solution.cpp](./tourist.cpp)

<img width="500" alt="image" src="https://github.com/krist7599555/pretoi19/assets/19445033/e69a1365-1ab6-40da-9cad-01d075adbb66">

## Oveserve 1 - กลุ่มทั้งหมดมีไม่เกิน 224 กลุ่ม

ข้อนี้ถ้าต้องทำตรงๆทั้งหมดเลยจะดูยากมาก แต่เราเริ่มจาก input ว่าเข้าให้ `group_ids <= 100,000`. แต่ถ้าเรามาคิดดูว่ามันสามารถมีหลาย group ที่มีจำนวนคนเท่ากันได้ ถ้าจำนวนคนเท่ากัน ยังไงผลลัพท์ของคำถามก็จะเหมือนกัน เราไม่จำเป็นต้องคำนวนซ้ำ

เลยหาจำนวนมากที่สุดที่จำนวนคนในกลุ่มไม่เท่ากันเลย เช่น

```python
กลุ่มที่มี 1 คน 1 กลุ่ม
กลุ่มที่มี 2 คน 1 กลุ่ม
กลุ่มที่มี 3 คน 1 กลุ่ม
กลุ่มที่มี 4 คน 1 กลุ่ม
กลุ่มที่มี 5 คน 1 กลุ่ม
กลุ่มที่มี 6 คน 1 กลุ่ม
กลุ่มที่มี 7 คน 1 กลุ่ม
..
กลุ่มที่มี n คน 1 กลุ่ม
```

คำนวนแล้ว เราจะมี จำนวนกลุ่ม(n) มากสุดไม่เกิน 224 `~sqrt(n)`

> ```cpp
> 1+2+3+4+5...+n <= 100,000
> n*(n+1)/2      <= 100,000
> n              <= 223.60679775
> ```

จากที่เราเคยต้องคิดแก้ปัญหาเป็นช่วง `L[i], R[i]` เราก็แปลงเป็นแก้ปัญหาเป็น จุดใดๆแทนไม่เกิน 224 ครั้ง

## Observe 2 - เช็คว่า node เดินไปถึงกันได้ไหมไวๆ

การจะเช็คว่ากราฟต่อกันไหมโดยเริ่มจากกราฟเต็มๆทุกเส้น แล้วค่อยๆตัดออก มันยากไป. ก็ให้คิดในมุมกลับ เริ่มจากกราฟตอนจบที่มันถูกแบ่งเป็นกลุ่มๆ แล้วค่อยๆรวมกราฟขึ้นไปเรื่อยๆเรื่อยๆจนกราฟทั้งหมดเป็นกลุ่มเดียว

> ส่วนตัวผมเรียก technique ที่รับคำถามทั้งหมดมาก่อน แล้วนำมาประมวนผล สร้างข้อมูลย้อนกลับจากผลลัพท์สุดท้ายว่า `Offline Algorithm` หรือ `Reverse Engineer` แต่ใครจะเรียกยังไงก็ได้นะ

เมื่อเราแปลงโจทย์จากการตัดเส้นแล้วถาม connectivity เป็น ต่อเส้นแล้วถาม connectivity ก็จะตรงกับ concept ของ set-union เลย

## Observe 3 - รวมของด้วย DSU (Disjoint Set Union)

> [Disjoint Set Unions - Union Find Algorith Tutorial](https://cp-algorithms.com/data_structures/disjoint_set_union.html)

<img width="500" alt="image" src="https://github.com/krist7599555/pretoi19/assets/19445033/99c0703b-4fcf-4db2-a286-a305d5137de5">

1. ให้เราเริ่มต้นกราฟด้วย `dsu` ที่ไม่มี node ไหนเชื่อมหากันเลย
1. ต่อเส้นเชื่อม ที่**ไม่มี**การแก้ไข policy `L[i], R[i]` ลงใน `dsu`
1. ทำจากหลังมาหน้า (backward)
   1. `cmd=1` ค่อยๆเพิ่มเส้นเชื่อมที่ตรงเงือนไขลงไป
   1. `cmd=2` เก็บคำตอบไว้ว่า `u, v` เดินไปหากันได้

ทำสิ่งนี้ 1 ครั้งใช้เวลาเท่า `O(Edge)`

## Time Complexity

- นับจำนวนกลุ่ม `O(100,000)` ด้วย bucket sort
- แบ่งกลุ่มตามจำนวนคน `O(100,000)`
- แก้ปัญหา 224 กลุ่ม `O(sqrt(GroupIds))`
  - แต่ละกลุ่ม สร้าง `dsu` และทำทั้งหมด `O(Edge)` ครั้ง
  - `dsu.merge` ใช้เวลา `O(log Node)`

รวม

```python
= O(100,000 + sqrt(GroupIds) * Edge * log Node)
= O(sqrt(GroupIds) * Edge * log Node)
```
