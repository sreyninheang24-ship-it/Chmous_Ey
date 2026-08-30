# Individual Exit Ticket — Record Architect

**Invariant I owned:**
"Each Team's ID, name, score, and missions move together as one object."
I enforced this by keeping all four fields inside a single `Team` struct
and making sure every operation that adds, deletes, sorts, or loads a
record copies/shifts/swaps the *whole struct* in one statement
(`teams[i] = teams[i+1]`, `Team tmp = teams[j]; ...`), never touching one
field independently of the others.

**Bug found:**
The file-parsing code (`parseLine`) mapped the `|`-separated fields to the
struct in the wrong order — it read `score` and `missions` from the wrong
columns, so loading `101|Byte Bandits|120|2` silently produced a team
named `"2"` with score `0`. This was caught by T9 (malformed file test)
when the displayed leaderboard didn't match the source file. Fixed by
re-ordering the assignment to `id, name, score, missions` to match the
documented file format.

**Next improvement:**
Add a dedicated unit-test harness (separate from the interactive menu)
that calls `addTeam`, `deleteTeam`, `loadTeams`, etc. directly with
hard-coded inputs and asserts on the results, instead of driving
everything through simulated keyboard input. That would catch
field-order bugs like the one above immediately, without needing to read
printed leaderboard output by eye.
