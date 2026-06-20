# Reference Verification List (web sources only, no textbooks)

> **Purpose:** for **my own verification** of the sources — checking each is real, valid,
> and openable. **NOT** part of the assignment deliverable.
>
> **Scope:** web tutorial articles only; no textbooks; no language-library documentation
> (cppreference / Mersenne Twister paper dropped because they document implementation
> tools, not algorithms — this is an algorithms-focused report).
>
> **Status legend:** ✅ confirmed live by automated check (2026-06-20) · ⚠️ not auto-confirmed, open/verify manually

---

## References (APA 7, web sources + course handout)

1. GeeksforGeeks. (n.d.). *Hashing | Set 3 (Open Addressing)*. Retrieved June 20, 2026, from https://www.geeksforgeeks.org/hashing-set-3-open-addressing/
   - ⚠️ open in browser — verify slug still resolves (GeeksforGeeks sometimes renames pages)

2. GeeksforGeeks. (n.d.). *Heap Sort – Data Structures and Algorithms Tutorials*. Retrieved June 20, 2026, from https://www.geeksforgeeks.org/heap-sort/
   - ⚠️ open in browser — confirm covers Floyd build + sift-down + O(n log n)

3. GeeksforGeeks. (n.d.). *Introduction to AVL Tree*. Retrieved June 20, 2026, from https://www.geeksforgeeks.org/introduction-to-avl-tree/
   - ⚠️ open in browser — confirm covers balance factor + O(log n) operations

4. GeeksforGeeks. (n.d.). *Radix Sort – Data Structures and Algorithms Tutorials*. Retrieved June 20, 2026, from https://www.geeksforgeeks.org/radix-sort/
   - ⚠️ open in browser — confirm covers LSD radix and O(d(n+b)) complexity

5. Programiz. (n.d.). *Radix Sort Algorithm*. Retrieved June 20, 2026, from https://www.programiz.com/dsa/radix-sort
   - ⚠️ open in browser — backup source for radix sort

6. W3Schools. (n.d.). *DSA Hash Tables*. Retrieved June 20, 2026, from https://www.w3schools.com/dsa/dsa_theory_hashtables.php
   - ⚠️ open in browser — confirm covers linear probing

7. W3Schools. (n.d.). *DSA Heap Sort*. Retrieved June 20, 2026, from https://www.w3schools.com/dsa/dsa_algo_heapsort.php
   - ⚠️ open in browser — confirm covers max-heap heap sort

8. W3Schools. (n.d.). *DSA Radix Sort*. Retrieved June 20, 2026, from https://www.w3schools.com/dsa/dsa_algo_radixsort.php
   - ⚠️ open in browser — confirm covers LSD radix sort

9. Wikipedia contributors. (n.d.). *AVL tree*. Wikipedia. Retrieved June 20, 2026, from https://en.wikipedia.org/wiki/AVL_tree
   - ⚠️ open in browser — backup source for §6 (other AVL ref is GeeksforGeeks)

10. Multimedia University. (2026). *CCP6214 Algorithm Design and Analysis — Group Assignment Specification (Trimester 2610)*. [Course handout].
    - ✅ have local copy (planning_docs/CCP6214_Assignment.pdf)

---

## Verification checklist

- [ ] #1 GeeksforGeeks Open Addressing opens; content matches what §5 of the report claims
- [ ] #2 GeeksforGeeks Heap Sort opens; content matches §4
- [ ] #3 GeeksforGeeks AVL Tree opens; content matches §6
- [ ] #4 GeeksforGeeks Radix Sort opens; content matches §3
- [ ] #5 Programiz Radix Sort opens
- [ ] #6 W3Schools Hash Tables opens
- [ ] #7 W3Schools Heap Sort opens
- [ ] #8 W3Schools Radix Sort opens
- [ ] #9 Wikipedia AVL tree opens
- [ ] #10 MMU course handout in our planning_docs folder

---

## Notes

- **No textbooks** — per group decision, references are web-only (plus the course handout).
- **No language-library docs** — cppreference and the Mersenne Twister paper were dropped
  because they document the timer/RNG we used, not the algorithms being analyzed.
- **Wikipedia caveat** — Wikipedia is academically weaker than peer-reviewed sources;
  some tutors do not accept it. We only cite Wikipedia for AVL (§6), and as a backup
  alongside GeeksforGeeks. If marks matter and the tutor is strict, replace Wikipedia
  with the CCP6214 lecture slides for §6.
- **GeeksforGeeks / W3Schools caveat** — APA 7 strictly requires lettered disambiguation
  when citing multiple pages from the same site (e.g. `GeeksforGeeks, n.d.-a` and
  `GeeksforGeeks, n.d.-b`). The current report uses just `(GeeksforGeeks, n.d.)`
  throughout — common in undergrad work but not strict APA. Switch to lettered form
  if the tutor enforces APA 7 rigorously.
