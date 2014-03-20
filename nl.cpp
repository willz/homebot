#include <string>
#include <iostream>
#include <algorithm>
#include <list>
#include <cstring>
#include <cctype>
#include <unordered_set>
#include <cassert>


using namespace std;

string task_words[] = {"give", "put", "go", "pick", "take", "open", "close"};
string obj_words[] = {"human", "plant", "couch", "chair", "sofa",
    "bed", "table", "workspace", "worktable", "teapoy", "desk", "television",
    "airconditioner", "washmachine", "closet", "cupboard", "refrigerator",
    "microwave", "book", "can", "remotecontrol", "bottle", "cup"};
string color_words[] = {"white", "black", "red", "green", "yellow", "blue"};

static unordered_set<string> colors;
static unordered_set<string> sorts;

bool find_word(const list<string>& words, const string& word) {
    for (auto w : words) {
        if (w == word) {
            return true;
        }
    }
    return false;
}

class SimpleObject {
public:
    string color;
    string sort;

    string toString(string var = "X") const {
        string ret;
        if (!color.empty()) {
            ret += "(color " + var + " " +  color + ") ";
        }
        if (!sort.empty()) {
            ret += "(sort " + var + " " + sort + ") ";
        }
        return ret;
    }
};

SimpleObject get_one_object(const list<string>& words) {
    SimpleObject o;
    for (auto w : words) {
        if (colors.count(w)) {
            o.color = w;
        }
        if (sorts.count(w)) {
            o.sort = w;
        }
    }
    return o;
}
void get_object(const list<string>& words, list<SimpleObject>& objs) {
    auto it_begin = words.begin();
    decltype(it_begin) it;
    while ((it = std::find(it_begin, words.end(), "and")) != words.end() ||
            (it = std::find(it_begin, words.end(), "or")) != words.end()) {
        SimpleObject o = get_one_object(list<string>(it_begin, it));
        if (o.sort != "") {
            objs.push_back(o);
        }
        it_begin = ++it;
    }
    auto o = get_one_object(list<string>(it_begin, words.end()));
    objs.push_back(o);
}

void parse_task(const list<string>& words, vector<string>& tasks) {
    if (find_word(words, "pick")) {
        list<SimpleObject> objs;
        get_object(words, objs);
        for (auto o : objs) {
            string t = "(:task (pickup X) (:cond " + o.toString() + "))";
            tasks.push_back(t);
        }
    } else if (find_word(words, "go")) {
        list<SimpleObject> objs;
        get_object(words, objs);
        for (auto o : objs) {
            string t = "(:task (goto X) (:cond " + o.toString() + "))";
            tasks.push_back(t);
        }
    } else if (find_word(words, "open")) {
        list<SimpleObject> objs;
        get_object(words, objs);
        for (auto o : objs) {
            string t = "(:task (open X) (:cond " + o.toString() + "))";
            tasks.push_back(t);
        }
    } else if (find_word(words, "close")) {
        list<SimpleObject> objs;
        get_object(words, objs);
        for (auto o : objs) {
            string t = "(:task (close X) (:cond " + o.toString() + "))";
            tasks.push_back(t);
        }
    } else if (find_word(words, "take")) {
        auto it = find(words.begin(), words.end(), "from");
        decltype(it) it1;
        if ((it1 = std::find(words.begin(), words.end(), "of")) != words.end()) {
            it = it1;
        }
        list<SimpleObject> big_objs;
        get_object(list<string>(it, words.end()), big_objs);
        list<SimpleObject> small_objs;
        get_object(list<string>(words.begin(), it), small_objs);
        for (auto b : big_objs) {
            for (auto s : small_objs) {
                string t = "(:task (takeout X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                tasks.push_back(t);
            }
        }
    } else if (find_word(words, "put")) {
        if (find_word(words, "on") || find_word(words, "next") || find_word(words, "near")
                || find_word(words, "to")) {
            // put on
            auto it = find(words.begin(), words.end(), "on");
            decltype(it) it1;
            if ((it1 = std::find(words.begin(), words.end(), "to")) != words.end()) {
                it = it1;
            }
            if ((it1 = std::find(words.begin(), words.end(), "near")) != words.end()) {
                it = it1;
            }
            list<SimpleObject> big_objs;
            get_object(list<string>(it, words.end()), big_objs);
            list<SimpleObject> small_objs;
            get_object(list<string>(words.begin(), it), small_objs);
            for (auto b : big_objs) {
                for (auto s : small_objs) {
                    string t = "(:task (puton X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                    tasks.push_back(t);
                }
            }
        } else if (find_word(words, "down")) {
            // put down
            list<SimpleObject> objs;
            get_object(words, objs);
            cout << "down " << objs.size() << endl;
            for (auto o : objs) {
                string t = "(:task (putdown X) (:cond " + o.toString() + "))";
                tasks.push_back(t);
            }
        } else if (find_word(words, "in")) {
            // put in
            auto it = find(words.begin(), words.end(), "in");
            list<SimpleObject> big_objs;
            get_object(list<string>(it, words.end()), big_objs);
            list<SimpleObject> small_objs;
            get_object(list<string>(words.begin(), it), small_objs);
            for (auto b : big_objs) {
                for (auto s : small_objs) {
                    string t = "(:task (putin X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                    tasks.push_back(t);
                }
            }
        } else if (find_word(words, "into")) {
            // put in
            auto it = find(words.begin(), words.end(), "into");
            list<SimpleObject> big_objs;
            get_object(list<string>(it, words.end()), big_objs);
            list<SimpleObject> small_objs;
            get_object(list<string>(words.begin(), it), small_objs);
            for (auto b : big_objs) {
                for (auto s : small_objs) {
                    string t = "(:task (putin X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                    tasks.push_back(t);
                }
            }
        }
    } else if (find_word(words, "give")) {
        if (find_word(words, "to")) {
            auto it = find(words.begin(), words.end(), "to");
            list<SimpleObject> big_objs;
            get_object(list<string>(it, words.end()), big_objs);
            list<SimpleObject> small_objs;
            get_object(list<string>(words.begin(), it), small_objs);
            for (auto b : big_objs) {
                for (auto s : small_objs) {
                    string t = "(:task (give X Y) (:cond " + b.toString() + s.toString("Y") + "))";
                    tasks.push_back(t);
                }
            }
        } else {
            auto it = find(words.begin(), words.end(), "human");
            list<SimpleObject> small_objs;
            get_object(list<string>(++it, words.end()), small_objs);
            for (auto o : small_objs) {
                string t = "(:task (give X Y) (:cond (sort X human) " + o.toString("Y") + "))";
                tasks.push_back(t);
            }
        }
    }
}

struct PrepPhase {
    string prep;
    string sort;
};

PrepPhase get_one_prepphase(const list<string>& words) {
    PrepPhase p;
    for (const auto& w : words) {
        if (sorts.count(w)) {
            p.sort = w;
        }
        if (w == "in" || w == "inside") {
            p.prep = "inside";
        } else if (w == "on") {
            p.prep = "on";
        } else if (w == "near" || w == "next") {
            p.prep = "near";
        } else if (w == "at") {
            p.prep = "at";
        }
    }
    return p;
}

void parse_info(list<string> words, vector<string>& infos) {
    if (find_word(words, "plate")) {
        list<SimpleObject> objs;
        get_object(words, objs);
        for (const auto o : objs) {
            infos.push_back("(:info (plate X) (:cond "+ o.toString() + "))");
        }
        return;

    }
    if (find_word(words, "close") || find_word(words, "closed")) {
        list<SimpleObject> big_objs;
        get_object(words, big_objs);
        for (const auto o : big_objs) {
            infos.push_back("(:info (closed X) (:cond "+ o.toString() + "))");
        }
        return;
    }
    if (find_word(words, "open") || find_word(words, "opened")) {
        list<SimpleObject> big_objs;
        get_object(words, big_objs);
        for (const auto o : big_objs) {
            infos.push_back("(:info (opened X) (:cond "+ o.toString() + "))");
        }
        return;
    }
    auto it = words.begin();
    for ( ; it != words.end(); ++it) {
        if (*it == "in" || *it == "on" || *it == "near" || *it == "next"
                || *it == "inside" || *it == "at") {
            break;
        }
    }
    list<SimpleObject> small_objs;
    get_object(list<string>(words.begin(), it), small_objs);

    auto prep_phase = list<string>(it, words.end());
    auto pit_begin = prep_phase.begin();
    decltype(pit_begin) pit;
    while ((pit = std::find(pit_begin, prep_phase.end(), "and")) != prep_phase.end() ||
            (pit = std::find(pit_begin, prep_phase.end(), "or")) != prep_phase.end()) {
        auto p = get_one_prepphase(list<string>(pit_begin, pit));
        pit_begin = ++pit;
        for (auto s : small_objs) {
            string t = "(:info (" + p.prep + "X Y) (:cond " + s.toString() + " (sort Y " + p.sort + ")))";
            infos.push_back(t);
        }
    }
    auto p = get_one_prepphase(list<string>(pit_begin, prep_phase.end()));
    for (auto s : small_objs) {
        string t = "(:info (" + p.prep + " X Y) (:cond " + s.toString() + " (sort Y " + p.sort + ")))";
        infos.push_back(t);
    }

    /*
    if (find_word(words, "on")) {
        auto it = find(words.begin(), words.end(), "on");
        list<SimpleObject> big_objs;
        get_object(list<string>(it, words.end()), big_objs);
        list<SimpleObject> small_objs;
        get_object(list<string>(words.begin(), it), small_objs);
        for (auto b : big_objs) {
            for (auto s : small_objs) {
                string t = "(:info (on X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                infos.push_back(t);
            }
        }
    } else if (find_word(words, "in")) {
        auto it = find(words.begin(), words.end(), "in");
        list<SimpleObject> big_objs;
        get_object(list<string>(it, words.end()), big_objs);
        list<SimpleObject> small_objs;
        get_object(list<string>(words.begin(), it), small_objs);
        for (auto b : big_objs) {
            for (auto s : small_objs) {
                string t = "(:info (inside X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                infos.push_back(t);
            }
        }
    } else if (find_word(words, "inside")) {
        auto it = find(words.begin(), words.end(), "inside");
        list<SimpleObject> big_objs;
        get_object(list<string>(it, words.end()), big_objs);
        list<SimpleObject> small_objs;
        get_object(list<string>(words.begin(), it), small_objs);
        for (auto b : big_objs) {
            for (auto s : small_objs) {
                string t = "(:info (inside X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                infos.push_back(t);
            }
        }
    } else if (find_word(words, "near")) {
        auto it = find(words.begin(), words.end(), "near");
        list<SimpleObject> big_objs;
        get_object(list<string>(it, words.end()), big_objs);
        list<SimpleObject> small_objs;
        get_object(list<string>(words.begin(), it), small_objs);
        for (auto b : big_objs) {
            for (auto s : small_objs) {
                string t = "(:info (near X Y) (:cond " + s.toString() + b.toString("Y") + "))";
                infos.push_back(t);
            }
        }
    }
    */
}

bool before_sort(const list<string>& words) {
    for (const auto& w : words) {
        if (sorts.count(w)) {
            return false;
        }
        if (w == "open" || w == "close") {
            return true;
        }
    }
    assert(false);
}

string nl_to_ins(const string& str) {
    if (colors.empty()) {
        colors.insert(color_words, color_words + sizeof(color_words) / sizeof(string));
        cout << colors.size() << endl;
    }
    if (sorts.empty()) {
        sorts.insert(obj_words, obj_words + sizeof(obj_words) / sizeof(string));
        cout << sorts.size() << endl;
    }
    string nl = str;
    std::transform(str.begin(), str.end(), nl.begin(), ::tolower);
    size_t start_pos = 0;
    while ((start_pos = nl.find(" me ", start_pos)) != string::npos) {
        nl.replace(start_pos, 4, " human ");
        start_pos += 7;
    }
    cout << nl << endl;

    size_t idx;
    list<string> words;
    string ins_str = "(:ins ";
    while ((idx = nl.find_first_of('.')) != string::npos) {
        string line = nl.substr(0, idx);
        cout << line << endl;
        words.clear();
        nl = nl.substr(idx + 1);
        char copy[256];
        strcpy(copy, line.c_str());
        char* p = strtok(copy, " ,.'");
        while (p) {
            words.push_back(string(p));
            p = strtok(NULL, " ,.'");
        }
        if (find_word(words, "must") || find_word(words, "cannot") || find_word(words, "not")) {
            // constrain
            string prefix;
            if (find_word(words, "must") && (!find_word(words, "cannot")) && !find_word(words, "not")) {
                prefix = "(:cons_notnot ";
            } else {
                prefix = "(:cons_not ";
            }
            if (find_word(words, "close") || find_word(words, "open")) {
                if (before_sort(words)) {
                    // it is task
                    vector<string> tasks;
                    parse_task(words, tasks);
                    for (const auto& t : tasks) {
                        string ret = "(:cons_not " + t + ")";
                        ins_str += ret;
                    }
                } else {
                    // it is info
                    vector<string> infos;
                    parse_info(words, infos);
                    for (const auto& i : infos) {
                        ins_str += prefix + i + ")";
                    }
                }

            }
            else if (find_word(words, "put") || find_word(words, "pick") || find_word(words, "go") ||
                    find_word(words, "give")) {
                // task
                vector<string> tasks;
                parse_task(words, tasks);
                for (const auto& t : tasks) {
                    string ret = "(:cons_not " + t + ")";
                    cout << "debug " << ret << endl;
                    ins_str += ret;
                }
            } else {
                // info
                vector<string> infos;
                parse_info(words, infos);
                for (const auto& i : infos) {
                    ins_str += prefix + i + ")";
                }
            }
        } else if (find_word(words, "open") || find_word(words, "close")) {
            cout << "open here " << endl;
            if (before_sort(words)) {
                // task
                vector<string> tasks;
                parse_task(words, tasks);
                for (const auto& t : tasks) {
                    ins_str += t;
                }
            } else {
                // info
                vector<string> infos;
                parse_info(words, infos);
                for (const auto& i : infos) {
                    ins_str += i;
                }
            }
        } else if (find_word(words, "put") || find_word(words, "pick") ||
                find_word(words, "give") || find_word(words, "go") ||
                find_word(words, "take")) {
            // task
            cout << "is task " << endl;
            vector<string> tasks;
            parse_task(words, tasks);
            for (auto t : tasks) {
                ins_str += t;
            }
        } else {
            vector<string> infos;
            parse_info(words, infos);
            for (const auto& i : infos) {
                ins_str += i;
            }
            // info
        }
    }
    return ins_str + ")";
}
