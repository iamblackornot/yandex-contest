## D. Лучший отдых

Ограничение времени	2 секунды

Ограничение памяти	256Mb

Ввод	стандартный ввод или input.txt

Вывод	стандартный вывод или output.txt

---

Васе предстоит выполнить n дел, для каждого дела он определил направление деятельности, заданное целым числом ai​.

За один день Вася может сделать любое количество дел, но он считает, что лучший вид отдыха — это смена деятельности. Поэтому он не хочет делать похожие дела в один и тот же день. Дела с номерами ai​ и aj​ считаются похожими, если разница между их номерами не превышает заданного коэффициента разнообразия k, то есть если ∣ai−aj∣≤k.

Также Вася хочет сделать все свои дела как можно быстрее. Например, если Васе необходимо сделать три дела с направлениями деятельности 1, 2 и 4 при k=2, то в первый день можно сделать дела 1 и 4, а во второй — 2. Сделать все дела за один день нельзя потому что направления деятельности 1 и 2 похожи.

Определите минимальное количество дней, необходимое Васе для выполнения всех его дел.
Формат ввода

В первой строке ввода через пробел даны два целых числа n и k — количество дел и коэффициент разнообразия ( 1≤n≤2⋅10^5, 0≤k≤10^9).

Во второй строке через пробел перечислены nn чисел ai​ — направления дел ( 1≤ai≤10^9).
Формат вывода

Выведите единственное целое число — минимальное количество дней, необходимое Васе для выполнения всех дел.

---

Пример 1

3 2
4 2 1


2

Пример 2

9 2
3 8 5 7 1 2 4 9 6


3

Пример 3

3 0
1 3 1
	

2

Пример 4

4 4
32 77 1 100

	
1

---

Примечания

Пояснения к первому примеру даны в условии.

Во втором примере Вася может выполнить в первый день дела 1, 4, 7, во второй — 2, 5, 8, а в третий — 3, 6, 9.

В третьем примере дела с направлением 1 должны быть сделаны в разные дни, а дело с направлением 3 может быть выполнено в любой из дней.

В четвертом примере все дела можно сделать за один день.