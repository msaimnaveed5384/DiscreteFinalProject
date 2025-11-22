#include "Functions.h"
#include "University.h"

using namespace std;

// ---------------- FunctionMapping methods ----------------

void FunctionMapping::addPair(const string& from, const string& to) {
    mapping.push_back({ from, to });
}

const vector<pair<string, string>>& FunctionMapping::getPairs() const {
    return mapping;
}

void FunctionMapping::print(const string& name) const {
    cout << name << " = { ";
    for (size_t i = 0; i < mapping.size(); ++i) {
        cout << "(" << mapping[i].first << " -> " << mapping[i].second << ")";
        if (i + 1 < mapping.size()) cout << ", ";
    }
    cout << " }\n";
}

bool FunctionMapping::isFunction() const {
    // A function cannot map the same domain element to two different codomain elements
    for (size_t i = 0; i < mapping.size(); ++i) {
        for (size_t j = i + 1; j < mapping.size(); ++j) {
            if (mapping[i].first == mapping[j].first &&
                mapping[i].second != mapping[j].second) {
                return false; // same input, different outputs
            }
        }
    }
    return true;
}

bool FunctionMapping::isTotal(const vector<string>& domain) const {
    // Every element of domain must appear at least once as "from"
    for (const string& d : domain) {
        bool found = false;
        for (const auto& p : mapping) {
            if (p.first == d) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

bool FunctionMapping::isInjective() const {
    // No two domain elements map to the same codomain value
    for (size_t i = 0; i < mapping.size(); ++i) {
        for (size_t j = i + 1; j < mapping.size(); ++j) {
            if (mapping[i].second == mapping[j].second &&
                mapping[i].first != mapping[j].first) {
                return false;
            }
        }
    }
    return true;
}

bool FunctionMapping::isSurjective(const vector<string>& codomain) const {
    // Every element in codomain must appear as some "to" value
    for (const string& c : codomain) {
        bool found = false;
        for (const auto& p : mapping) {
            if (p.second == c) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

bool FunctionMapping::isBijective(const vector<string>& domain,
    const vector<string>& codomain) const {
    return isFunction() && isTotal(domain) && isInjective() && isSurjective(codomain);
}

FunctionMapping FunctionMapping::compose(const FunctionMapping& g) const {
    // this: f: A -> B, g: B -> C  => result: h = g ∘ f : A -> C
    FunctionMapping result;

    for (const auto& p1 : mapping) {
        const string& a = p1.first;
        const string& b = p1.second;

        // find g(b) = c
        for (const auto& p2 : g.getPairs()) {
            if (p2.first == b) {
                const string& c = p2.second;
                result.addPair(a, c); // h(a) = c
            }
        }
    }

    return result;
}

FunctionMapping FunctionMapping::inverse() const {
    FunctionMapping inv;
    for (const auto& p : mapping) {
        inv.addPair(p.second, p.first);
    }
    return inv;
}

// ---------------- FunctionsModule builders ----------------

// Student -> Course (primary course):
// For each student, pick the first course they are enrolled in.
FunctionMapping FunctionsModule::buildStudentToCourseFunction(const University& uni) {
    FunctionMapping f;

    vector<string> courseCodes = uni.getAllCourseCodes();
    set<string> assignedStudents; // to ensure one course per student

    for (const string& courseCode : courseCodes) {
        vector<string> students = uni.getStudentsInCourse(courseCode);
        for (const string& sid : students) {
            if (assignedStudents.find(sid) == assignedStudents.end()) {
                f.addPair(sid, courseCode);
                assignedStudents.insert(sid);
            }
        }
    }

    return f;
}

// Course -> Faculty:
// Use facultyCourses mapping. If multiple faculty teach same course, first one wins,
// but isFunction() will still detect if there are conflicting mappings.
FunctionMapping FunctionsModule::buildCourseToFacultyFunction(const University& uni) {
    FunctionMapping f;
    vector<pair<string, string>> fcPairs = uni.getFacultyCoursePairs();
    // fcPairs: (facultyId, courseCode)
    set<string> assignedCourses;

    for (const auto& p : fcPairs) {
        const string& facultyId = p.first;
        const string& courseCode = p.second;

        if (assignedCourses.find(courseCode) == assignedCourses.end()) {
            f.addPair(courseCode, facultyId); // course -> faculty
            assignedCourses.insert(courseCode);
        }
        else {
            // if we needed to detect multiple faculty per course, we could add
            // an extra pair here, but isFunction() can be used on raw data too.
        }
    }

    return f;
}

// Faculty -> Room:
// Derived from courseRooms and courseToFaculty.
// For each faculty, take the first room among the courses they teach.
FunctionMapping FunctionsModule::buildFacultyToRoomFunction(const University& uni) {
    FunctionMapping f;

    vector<pair<string, string>> fcPairs = uni.getFacultyCoursePairs(); // (faculty, course)
    vector<pair<string, string>> crPairs = uni.getCourseRoomPairs();    // (course, room)

    // For quick lookup: course -> room (they already exist, but we reuse vector)
    // We will just do nested loops (small size in typical projects).
    set<string> assignedFaculty;

    for (const auto& fc : fcPairs) {
        const string& facultyId = fc.first;
        const string& courseCode = fc.second;

        if (assignedFaculty.find(facultyId) != assignedFaculty.end()) {
            continue; // already assigned a main room
        }

        // find room of this course
        for (const auto& cr : crPairs) {
            if (cr.first == courseCode) {
                const string& roomId = cr.second;
                f.addPair(facultyId, roomId); // faculty -> room
                assignedFaculty.insert(facultyId);
                break;
            }
        }
    }

    return f;
}

// ---------------- Analysis helpers ----------------

// Dry run focus: Course -> Faculty function
void FunctionsModule::analyzeCourseFacultyFunction(const University& uni) {
    FunctionMapping f = buildCourseToFacultyFunction(uni);

    vector<string> allCourses = uni.getAllCourseCodes();
    vector<string> allFaculty = uni.getAllFacultyIds();

    cout << "=== Function f: Course -> Faculty ===\n";
    f.print("f(course)");

    bool func = f.isFunction();
    bool total = f.isTotal(allCourses);
    bool inject = f.isInjective();
    bool surject = f.isSurjective(allFaculty);
    bool biject = f.isBijective(allCourses, allFaculty);

    cout << "\nIs valid function (no course mapped to two faculty)? " << (func ? "Yes" : "No") << "\n";
    cout << "Is total on courses (every course has a faculty)?   " << (total ? "Yes" : "No") << "\n";
    cout << "Is injective (no two courses share same faculty)?   " << (inject ? "Yes" : "No") << "\n";
    cout << "Is surjective (every faculty teaches a course)?      " << (surject ? "Yes" : "No") << "\n";
    cout << "Is bijective (perfect 1-1 matching)?                 " << (biject ? "Yes" : "No") << "\n\n";

    cout << "Dry run interpretation:\n";
    if (func && total) {
        cout << "- Every course is assigned to exactly one faculty (function + total).\n";
    }
    else {
        if (!func) {
            cout << "- Some courses are assigned to more than one faculty (not a function).\n";
        }
        if (!total) {
            cout << "- Some courses have no faculty assigned.\n";
        }
    }
    cout << "- Injectivity here means each faculty teaches at most one course.\n";
    cout << "- Surjectivity means all faculty members are used in teaching.\n\n";
}

void FunctionsModule::analyzeStudentCourseFunction(const University& uni) {
    FunctionMapping f = buildStudentToCourseFunction(uni);

    vector<string> allStudents = uni.getAllStudentIds();
    vector<string> allCourses = uni.getAllCourseCodes();

    cout << "=== Function g: Student -> Primary Course ===\n";
    f.print("g(student)");

    bool func = f.isFunction();
    bool total = f.isTotal(allStudents);
    bool inject = f.isInjective();
    bool surject = f.isSurjective(allCourses);
    bool biject = f.isBijective(allStudents, allCourses);

    cout << "\nIs valid function (each student has <= 1 primary course)? " << (func ? "Yes" : "No") << "\n";
    cout << "Is total on students (each student has a primary course)?  " << (total ? "Yes" : "No") << "\n";
    cout << "Is injective (no two students share same primary course)?  " << (inject ? "Yes" : "No") << "\n";
    cout << "Is surjective (all courses used as a primary course)?      " << (surject ? "Yes" : "No") << "\n";
    cout << "Is bijective (perfect 1-1)?                                " << (biject ? "Yes" : "No") << "\n\n";
}

void FunctionsModule::analyzeFacultyRoomFunction(const University& uni) {
    FunctionMapping courseToFaculty = buildCourseToFacultyFunction(uni);
    FunctionMapping facultyToRoom = buildFacultyToRoomFunction(uni);

    cout << "=== Function h: Faculty -> Room ===\n";
    facultyToRoom.print("h(faculty)");

    vector<string> allFaculty = uni.getAllFacultyIds();
    // Build codomain of rooms actually used
    vector<pair<string, string>> crPairs = uni.getCourseRoomPairs();
    set<string> roomsSet;
    for (const auto& p : crPairs) {
        roomsSet.insert(p.second);
    }
    vector<string> allRooms(roomsSet.begin(), roomsSet.end());

    bool func = facultyToRoom.isFunction();
    bool total = facultyToRoom.isTotal(allFaculty);
    bool inject = facultyToRoom.isInjective();
    bool surject = facultyToRoom.isSurjective(allRooms);
    bool biject = facultyToRoom.isBijective(allFaculty, allRooms);

    cout << "\nIs valid function (each faculty has <= 1 main room)? " << (func ? "Yes" : "No") << "\n";
    cout << "Is total on faculty (each faculty has a room)?       " << (total ? "Yes" : "No") << "\n";
    cout << "Is injective (no two faculty share same room)?       " << (inject ? "Yes" : "No") << "\n";
    cout << "Is surjective (all rooms used by some faculty)?      " << (surject ? "Yes" : "No") << "\n";
    cout << "Is bijective (perfect 1-1)?                          " << (biject ? "Yes" : "No") << "\n\n";

    // Example of composition: Course -> Faculty -> Room
    FunctionMapping courseToRoomViaFaculty = courseToFaculty.compose(facultyToRoom);
    cout << "=== Composition: Course -> Faculty -> Room (k = h ∘ f) ===\n";
    courseToRoomViaFaculty.print("k(course)");
    cout << "\nInterpretation: k(c) gives the room of the faculty teaching course c.\n\n";
}