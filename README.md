# Chmous_Ey
algorithm assignment

=============== MEMBER ROLE ==================
Jully: UX desinger!!!!!!!!!!!!!!!
tep: Memory Guardian
NIN: presenter 
seng: Record Architect
Chheang: Test Captain

================= valid rule =======================
  ID must be positive and unique 
  name must not be empty and must fit
  the array; 
  score and missions must be nonnegative; 
  mission points entered through the menu must be from 1 through 100.

=================== Core Mission: Required Features ====================


A. Dynamic collection
Start with `teams = NULL`, `size = 0`, `capacity = 0`. Grow by doubling. Use a
temporary result for `realloc`. Update capacity only after success.
B. Register a team
Validate the complete candidate, reject duplicate ID, expand if necessary, copy the
complete record, then increment size.

------ find team by ID
C. Record mission points
Find the team by ID. Validate points first. On success, add points and increment
missions. Invalid input leaves the record unchanged.
D. Find a team
Return the matching logical index or −1. Do not search unused capacity.
E. Remove a team
Find by ID, shift complete `Team` objects left, and reduce size exactly once. Missing ID
leaves state unchanged.
F. Show leaderboard
Sort descending by score; use missions as the tie-breaker if possible. Swap complete
`Team` objects. Display only logical records.
G. Load safely

Read one line with `fgets`; parse into temporary fields; validate; reject malformed or
duplicate lines; commit only accepted complete records.
H. Save and clean up
Write all logical records in the documented format; check write and close results; `free`
the allocation exactly once and set the owner to `NULL`.
