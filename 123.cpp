//#include <bits/stdc++.h>
//
//using namespace std;
//// <delete> Удаление символа после текущей позиции курсора.
//// <bspace> Удаление символа перед текущей позицией курсора.
//// <left> Курсор перемещается влево на один символ.
//// <right> Курсор перемещается вправо на один символ.
//// hellochild
//// helto<left><bspace>l<delete>ochilds<bspace>
//
//
//string f(const string& s1, string s2){
//    string s3;
//    int ind = 0;
//    int n = s2.size();
//    for (int i = 0; i < n; ++i){
//        if (s2[i] == '<' and s2[i + 1] == 'l'){
//            if (ind > 0){
//                --ind;
//            }
//            i += 5;
//        }
//        else if (s2[i] == '<' and s2[i + 1] == 'r'){
//            if (ind < s3.size()){
//                ++ind;
//            }
//            i += 6;
//        }
//        else if (s2[i] == '<' and s2[i + 1] == 'b' ){
//            if (ind > 0) {
//                --ind;
//                s3.replace(ind, 1, "");
//            }
//            i += 7;
//        }
//        else if (s2[i] == '<' and s2[i + 1] == 'd' ){
//            if (ind < s3.size()){
//                s3.replace(ind, 1, "");
//            }
//            i += 7;
//        }
//        else{
//            string tmp(1, s2[i]);
//            s3.insert(ind, tmp);
//            ++ind;
//        }
//    }
//    return s1 == s3 ? "Yes" : "No";
//}
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(0);
//
//    string s1, s2;
//    cin >> s1 >> s2;
//
//    cout << f(s1, s2);
//}
//
