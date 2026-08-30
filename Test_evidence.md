# Test Evidence

## T1 - Empty Leaderboard
- Input: Show leaderboard when there are no teams.
- Expected: Display "No teams."
- Actual: "No teams." displayed.
- Result: PASS
- Defect exposed: Incorrect handling of empty collection.

## T2 - First Team
- Input: Add team ID 101, name Dragons.
- Expected: Team is added with score 0 and missions 0.
- Actual: Team registered successfully.
- Result: PASS
- Defect exposed: Incorrect team creation or size update.

## T3 - Growth Boundary
- Input: Add a third team when capacity is 2.
- Expected: Capacity grows and previous teams remain correct.
- Actual: Third team added and previous records preserved.
- Result: PASS
- Defect exposed: Incorrect dynamic-array growth or realloc.

## T4 - Duplicate ID
- Input: Add another team with ID 101.
- Expected: Duplicate ID is rejected and original record stays unchanged.
- Actual: Registration rejected.
- Result: PASS
- Defect exposed: Duplicate IDs being accepted.

## T5 - Invalid Points
- Input: Enter mission points 0 and 101.
- Expected: Both inputs are rejected and the record stays unchanged.
- Actual: Both invalid inputs were rejected.
- Result: PASS
- Defect exposed: Incorrect 1-100 validation.

## T6 - Search Absent Team
- Input: Search for ID 999.
- Expected: Team not found and no data changes.
- Actual: "Team not found."
- Result: PASS
- Defect exposed: Incorrect search logic.

## T7 - Delete Boundaries
- Input: Delete the first team and then the last team.
- Expected: Records shift correctly and size decreases.
- Actual: Teams removed and remaining records stayed correct.
- Result: PASS
- Defect exposed: Incorrect shifting or size handling.

## T8 - Sort Identity
- Input: Sort teams with different/equal scores.
- Expected: Highest score first; team information stays together.
- Actual: Correct ranking and team information stayed together.
- Result: PASS
- Defect exposed: Incorrect sorting or separated team fields.

## T9 - Malformed File
- Input: Load a file containing malformed, duplicate, and invalid records.
- Expected: Invalid records are rejected; valid records are loaded.
- Actual: Invalid records were rejected.
- Result: PASS
- Defect exposed: Unsafe file parsing or validation.

## T10 - Save/Reload
- Input: Save teams, exit, restart, and load teams.
- Expected: The same accepted records are restored.
- Actual: Saved records were restored correctly.
- Result: PASS
- Defect exposed: Save or load errors.

## T11 - Cleanup
- Input: Exit the program.
- Expected: Memory is freed and the pointer is reset.
- Actual: cleanup() frees the allocation and resets the values.
- Result: PASS
- Defect exposed: Memory leak or incorrect cleanup.

# Summary

Required tests passed: 11/11

Additional bug found:
Non-numeric input for Team ID is not handled properly.

Next improvement:
Add proper input validation for Team ID.
