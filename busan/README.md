# Busan

[🎉 solution.cpp](./busan.cpp)

<img width="519" alt="image" src="https://github.com/krist7599555/pretoi19/assets/19445033/37b4cc4f-c20b-48e3-8340-b9663859b097">

วิเคราะเคสแบบละตำแหน่งมีอยู่ 4 รูปแบบ
|left|right|action|
|-|-|-|
|zombie|zombie|ไม่รับข้ามเลย (easy)|
|human|human|รับได้เลย (easy)|
|human|zombie|ใส่ knapsack ซ้าย (hard)|
|zombie|human|ใส่ knapsack ขวา (hard)|

จากข้างบนจะเห็นว่า

- เมื่อรับ human-zombie จะมีเพียงประตูฝั่งขวาที่พัง และทำนองเดียวกัน
- เมื่อรับ zombie-human ก็มีเพียงประตูฝั่งซ้ายที่พัง

ดังนั้น **ประตูซ้ายกับขวาจึงไม่ได้เกี่ยวข้องกัน เลยสามารถแยกกันทำได้**

ถ้าไม่มีการสลับประตูเลย เราก็ทำ knapsack หาคนมาสุดที่ช่วยได้โดยกำแพงแต่ละด้านไม่พัง ก็ถือว่าตอบได้เลย

แต่เมื่อมีการสลับประตูเราก็ยังต้องหาการคำนวน knapsack ให้ได้ไวๆอยู่ เลยใช้วิธีทำ knapsack จากหน้าไปหลัง และหลังไปหน้า

| ประตู | sequence        |
| ----- | --------------- |
| left  | 0..n (forward)  |
| right | 0..n (forward)  |
| left  | n..0 (backward) |
| right | n..0 (backward) |

คำตอบคือ

```python
จำนวนคนมากสุดที่รับได้ = max { i = 0..N |

  maxhuman_start_left =
    + knapsack_left[0..i]
    + knapsack_right[i+1..n]; โดยกำแพงไม่พัง

  maxhuman_start_right =
    + knapsack_right[0..i]
    + knapsack_left[i+1..n]; โดยกำแพงไม่พัง

  return maxhuman_start_left + maxhuman_start_right
}
```

## Time Complexity

- ทำ Knapsack 4 ครั้ง `O(4 * N * W)`
- วนหาคำตอบตามจุดตัด และลองทุกน้ำหนักที่เป็นไปได้ `O(N * W)`
- รวม `O(N * W)`
