#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>

using namespace std;

static const string DB_PATH = "data.log";

/**
 * DB_PATH 라는 이름의 파일을 열어서
 * 그 파일의 맨 끝에 line과 개행 문자를 추가한다
 * 파일 쓰기가 잘 되었으면 true, 실패했으면 false를 반환한다.
 */
bool append_line(const string& line) { // string&를 사용하여 원본을 바로 참조함
    ofstream out(DB_PATH, ios::app); // (파일명, 옵션): 파일을 append 모드로 연다
    if (!out) return false;
    out << line << "\n"; // out 파일에 line과 개행 문자를 추가한다.
    return out.good(); // ofstream.good()은 정상적으로 작동했으면 true를 반환
}

/**
 * key-value를 입력받아 데이터 로그에 형식에 맞게 추가한다.
 * 성공 여부를 반환한다.
 */
bool kv_put(const string& key, const string& value) {
    // 현재는 탭으로 키 값을 구분하고 개행으로 아이템을 구분하므로, 키나 값에는 탭과 개행이 있어선 안 된다. 이를 검사하는 단계다.
    if (key.find('\n') != string::npos || key.find('\t') != string::npos) return false; // string::find(c)는 문자열 안에서 c 문자를 찾는다. 찾지 못하면 string::npos 라는 특수 값을 반환한다.
    if (value.find('\n') != string::npos || value.find('\t') != string::npos) return false;
    // P\tkey\tvalue 형식으로 저장될 한 줄을 작성한다.
    string line = "P\t" + key + "\t" + value;
    // 작성한 한 줄을 추가한다.
    return append_line(line);
}

/**
 * 삭제할 키 값을 입력받아 데이터 삭제 로그를 남긴다.
 * 성공 여부를 반환한다.
 */
bool kv_del(const string& key) {
    if (key.find('\n') != string::npos || key.find('\t') != string::npos) return false; // 마찬가지로 키에 개행과 탭이 있는지 검사한다.
    string line = "D\t" + key; // 데이터 삭제 로그를 남긴다.
    return append_line(line);
}

/**
 * 데이터 로그를 한 줄씩 끝까지 읽어가며 key 값을 찾는다.
 * 발견할 때마다 last_value로 업데이트한다.
 * 삭제 로그를 발견하면 last_value를 지운다.
 * 값이 존재한다면 그 값을 반환한다.
 */
optional<string> kv_get(const string& key) {
    ifstream in(DB_PATH);
    if (!in) return nullopt;

    string line;
    optional<string> last_value;

    // 로그에서 한 줄씩 읽어 온다.
    while (getline(in, line)) {
        if (line.empty()) continue;

        // parts[0]: P or D
        // parts[1]: key
        // parts[2]: value
        // 형식으로 나누어 저장한다.
        vector<string> parts;
        size_t start = 0;
        while (true) {
            size_t pos = line.find('\t', start); // start 위치부터 첫 번째 탭 문자의 인덱스를 반환한다.
            if (pos == string::npos) { // 더 이상 탭 문자가 없다면 마지막 토큰이다.
                parts.push_back(line.substr(start)); // 현재 토큰을 parts 벡터에 추가한다.
                break;
            }
            parts.push_back(line.substr(start, pos - start)); // 현재 토큰을 parts에 추가한다.
            start = pos + 1; // 시작 지점을 업데이트한다.
        }

        if (parts.size() < 2) continue; // 형식에 맞지 않는 경우 무시한다.
        string op = parts[0];
        string k = parts[1];

        if (k == key) { // 한 줄씩 읽어가면서 봤을 때 키 값이 내가 찾던 키 값이라면
            if (op == "P") { // 값이 쓰인 경우
                if (parts.size() >= 3) last_value = parts[2]; // parts[2]: value로 last_value를 업데이트 한다.
                else last_value = ""; // 값이 비어있는 경우 ""를 last_value로 한다.
            } else if (op == "D") { // 값이 삭제된 경우
                last_value.reset(); // last_value를 지운다.
            }
        }
    }
    return last_value; // 최종 값을 반환한다.
}

int main(int argc, char** argv) {
    // put 명령인 경우
    if (argc >= 4 && string(argv[1]) == "put") {
        bool ok = kv_put(argv[2], argv[3]);
        cout << (ok ? "OK\n" : "FAIL\n");
        return ok ? 0 : 1;
    }
    // get 명령인 경우
    if (argc >= 3 && string(argv[1]) == "get") {
        auto v = kv_get(argv[2]);
        if (v) cout << *v << "\n";
        else   cout << "(nil)\n";
        return 0;
    }
    // del 명령인 경우
    if (argc >= 3 && string(argv[1]) == "del") {
        bool ok = kv_del(argv[2]);
        cout << (ok ? "OK\n" : "FAIL\n");
        return ok ? 0 : 1;
    }
    // 잘못된 입력인 경우
    cout << "usage:\n"
            "  " << argv[0] << " put <key> <value>\n"
            "  " << argv[0] << " get <key>\n"
            "  " << argv[0] << " del <key>\n";
    return 0;
}