/*思路:
将需要进行匹配的字符串建成广义后缀自动机，并用set启发式合并维护其endpos集合，
这里的endpos是一个pair，包含endpos属于第几个字符串的第几位，输入串进行匹配时，走自动机的前向转移边，
根据公共子串的长度从长到短输出10个字符串(如果一个节点没有10个串就跳后缀连接，跳到空节点为止)
动态加入新字符串就直接extend
如果删除字符串，就先打删除标记，删除根号级别的字符串后就重构后缀自动机
*/
#include <bits/stdc++.h>
#include "SAMbuild.cpp"

using namespace std;

int main() {
    SAMbuild *SAM1 = new SAMbuild();
    SAM1->buildSam();
    for (int i = 1; i <= 5; ++i) {
        SAM1->findSam();
    }
    return 0;
}