#include "Relations.h"
#include "University.h"

using namespace std;

void Relation::addPair(const string& a, const string& b) {
    pairs.push_back({ a, b });
}

const vector<pair<string, string>>& Relation::getPairs() const {
    return pairs;
}

void Relation::print(const string& name) const {
    cout << name << " = { ";
    for (size_t i = 0; i < pairs.size(); ++i) {
        cout << "(" << pairs[i].first << ", " << pairs[i].second << ")";
        if (i + 1 < pairs.size()) {
            cout << ", ";
        }
    }
    cout << " }\n";
}

bool Relation::isReflexive(const vector<string>& universe) const {
    // For every x in universe, (x, x) must be in pairs
    for (const string& x : universe) {
        bool found = false;
        for (const auto& p : pairs) {
            if (p.first == x && p.second == x) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool Relation::isSymmetric() const {
    // For every (a, b) in R, (b, a) must also be in R
    for (const auto& p : pairs) {
        const string& a = p.first;
        const string& b = p.second;

        bool foundReverse = false;
        for (const auto& q : pairs) {
            if (q.first == b && q.second == a) {
                foundReverse = true;
                break;
            }
        }

        if (!foundReverse) {
            return false;
        }
    }
    return true;
}

bool Relation::isTransitive() const {
    // For all (a, b) and (b, c) in R, (a, c) must also be in R
    for (size_t i = 0; i < pairs.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            const string& a = pairs[i].first;
            const string& b = pairs[i].second;
            const string& b2 = pairs[j].first;
            const string& c = pairs[j].second;

            if (b == b2) {
                bool foundAC = false;
                for (const auto& r : pairs) {
                    if (r.first == a && r.second == c) {
                        foundAC = true;
                        break;
                    }
                }
                if (!foundAC) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Relation::isAntisymmetric() const {
    // For all (a, b) and (b, a) in R with a != b, relation is not antisymmetric
    for (const auto& p : pairs) {
        const string& a = p.first;
        const string& b = p.second;
        if (a == b) {
            continue;
        }
        for (const auto& q : pairs) {
            if (q.first == b && q.second == a) {
                return false;
            }
        }
    }
    return true;
}

bool Relation::isEquivalence(const vector<string>& universe) const {
    return isReflexive(universe) && isSymmetric() && isTransitive();
}

bool Relation::isPartialOrder(const vector<string>& universe) const {
    return isReflexive(universe) && isAntisymmetric() && isTransitive();
}

Relation Relation::compose(const Relation& other) const {
    Relation result;
    // this: R(a, b), other: S(b, c)
    for (const auto& p1 : pairs) {
        for (const auto& p2 : other.getPairs()) {
            if (p1.second == p2.first) {
                result.addPair(p1.first, p2.second); // (a, c)
            }
        }
    }
    return result;
}

// ----------------- RelationsModule -----------------

Relation RelationsModule::buildStudentCourseRelation(const University& uni) {
    Relation r;

    vector<string> courseCodes = uni.getAllCourseCodes();

    for (const string& courseCode : courseCodes) {
        vector<string> studentsInCourse = uni.getStudentsInCourse(courseCode);
        for (const string& sid : studentsInCourse) {
            // Relation: student -> course
            r.addPair(sid, courseCode);
        }
    }

    return r;
}

void RelationsModule::analyzeStudentCourseRelation(const University& uni) {
    Relation r = buildStudentCourseRelation(uni);
   
    cout << "=== Student to Course Relation (R) ===\n";
    r.print("R(student, course)");
    cout << "\n";

    // Universe choice: all student IDs (for property checks)
    vector<string> students = uni.getAllStudentIds();

    cout << "Reflexive on students?     " << (r.isReflexive(students) ? "Yes" : "No") << "\n";
    cout << "Symmetric?                 " << (r.isSymmetric() ? "Yes" : "No") << "\n";
    cout << "Transitive?                " << (r.isTransitive() ? "Yes" : "No") << "\n";
    cout << "Antisymmetric?             " << (r.isAntisymmetric() ? "Yes" : "No") << "\n";
    cout << "Equivalence relation?      " << (r.isEquivalence(students) ? "Yes" : "No") << "\n";
    cout << "Partial order on students? " << (r.isPartialOrder(students) ? "Yes" : "No") << "\n\n";
}

Relation RelationsModule::buildFacultyCourseRelation(const University& uni) {
    Relation r;
    vector<pair<string, string>> fcPairs = uni.getFacultyCoursePairs();
    for (const auto& p : fcPairs) {
        r.addPair(p.first, p.second); // faculty -> course
    }
    return r;
}

Relation RelationsModule::buildCourseRoomRelation(const University& uni) {
    Relation r;
    vector<pair<string, string>> crPairs = uni.getCourseRoomPairs();
    for (const auto& p : crPairs) {
        r.addPair(p.first, p.second); // course -> room
    }
    return r;
}

Relation RelationsModule::buildCourseConflictRelation(const University& uni) {
    Relation r;
    vector<pair<string, string>> ccPairs = uni.getCourseConflictPairs();
    for (const auto& p : ccPairs) {
        r.addPair(p.first, p.second); // course -> conflictingCourse
    }
    return r;
}

void RelationsModule::analyzeFacultyCourseRelation(const University& uni) {
    Relation r = buildFacultyCourseRelation(uni);
    cout << "=== Faculty to Course Relation (F) ===\n";
    r.print("F(faculty, course)");
    cout << "\n";

    // Build universe of faculty ids from relation pairs
    vector<pair<string, string>> fcPairs = uni.getFacultyCoursePairs();
    set<string> facultySet;
    for (const auto& p : fcPairs) {
        facultySet.insert(p.first);
    }
    vector<string> facultyUniverse(facultySet.begin(), facultySet.end());

    cout << "Reflexive on faculty?      " << (r.isReflexive(facultyUniverse) ? "Yes" : "No") << "\n";
    cout << "Symmetric?                 " << (r.isSymmetric() ? "Yes" : "No") << "\n";
    cout << "Transitive?                " << (r.isTransitive() ? "Yes" : "No") << "\n";
    cout << "Antisymmetric?             " << (r.isAntisymmetric() ? "Yes" : "No") << "\n";
    cout << "Equivalence relation?      " << (r.isEquivalence(facultyUniverse) ? "Yes" : "No") << "\n";
    cout << "Partial order on faculty?  " << (r.isPartialOrder(facultyUniverse) ? "Yes" : "No") << "\n\n";
}

void RelationsModule::analyzeCourseRoomRelation(const University& uni) {
    Relation r = buildCourseRoomRelation(uni);

    cout << "=== Course to Room Relation (CR) ===\n";
    r.print("CR(course, room)");
    cout << "\n";

    // Build universe of course codes from relation pairs
    vector<pair<string, string>> crPairs = uni.getCourseRoomPairs();
    set<string> courseSet;
    for (const auto& p : crPairs) {
        courseSet.insert(p.first);  // courseCode is the domain
    }
    vector<string> courseUniverse(courseSet.begin(), courseSet.end());

    cout << "Reflexive on courses?      "
        << (r.isReflexive(courseUniverse) ? "Yes" : "No") << "\n";

    cout << "Symmetric?                 "
        << (r.isSymmetric() ? "Yes" : "No") << "\n";

    cout << "Transitive?                "
        << (r.isTransitive() ? "Yes" : "No") << "\n";

    cout << "Antisymmetric?             "
        << (r.isAntisymmetric() ? "Yes" : "No") << "\n";

    cout << "Equivalence relation?      "
        << (r.isEquivalence(courseUniverse) ? "Yes" : "No") << "\n";

    cout << "Partial order on courses?  "
        << (r.isPartialOrder(courseUniverse) ? "Yes" : "No") << "\n\n";

}

void RelationsModule::detectIndirectStudentConflicts(const University& uni) {
    cout << "=== Detecting indirect student course conflicts ===\n";

    // R1: Student -> Course (from enrollments)
    Relation studentCourse = buildStudentCourseRelation(uni);

    // R2: Course -> ConflictingCourse (from admin input)
    Relation courseConflict = buildCourseConflictRelation(uni);

    if (courseConflict.getPairs().empty()) {
        cout << "No course conflicts have been defined yet.\n\n";
        return;
    }

    // Composition: Student -> ConflictingCourse
    Relation studentToConflict = studentCourse.compose(courseConflict);

    if (studentToConflict.getPairs().empty()) {
        cout << "No indirect conflicts detected for any student.\n\n";
        return;
    }

    studentToConflict.print("StudentToConflict(student, conflictingCourse)");
    cout << "\n";

    cout << "Interpretation:\n";
    cout << "- Each pair (s, c2) means student s is enrolled in some course c1\n";
    cout << "  that conflicts with c2. This is detected via relation composition.\n\n";
}