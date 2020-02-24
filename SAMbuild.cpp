/*思路:
将需要进行匹配的字符串建成广义后缀自动机，并用set启发式合并维护其endpos集合，
这里的endpos是一个pair，包含endpos属于第几个字符串的第几位，输入串进行匹配时，走自动机的前向转移边，
根据公共子串的长度从长到短输出10个字符串(如果一个节点没有10个串就跳后缀连接，跳到空节点为止)
动态加入新字符串就直接extend
如果删除字符串，就先打删除标记，删除根号级别的字符串后就重构后缀自动机
*/
#include <tr1/unordered_set>
#include <tr1/unordered_map>

struct SAMbuild {

    inline void R(int &v) {
        static char ch;
        v = 0;
        bool p = 0;
        do {
            ch = getchar();
            if (ch == '-') p = 1;
        } while (!isdigit(ch));
        while (isdigit(ch)) {
            v = (v + (v << 2) << 1) + (ch ^ '0');
            ch = getchar();
        }
        if (p) v = -v;
    }

//广义后缀自动机
    struct node {
        int fa, max;//fa代表一个节点的后缀连接，max代表一个节点内的字符串的maxlength
        std::tr1::unordered_map<int, int> ch;//ch代表一个节点的前向转移边
        node(int max, int fa) : max(max), fa(fa) {
        }

        node() {

        }
    };

    std::vector<node> tr;
    int last, cur, tot, rt;
    std::tr1::unordered_set<int> emptyUnorderSet;
    std::vector<std::tr1::unordered_set<int>> endPos;
    std::vector<std::vector<int>> q;
    int id;

//广义后缀自动机增量构造法
    inline int extend(int p, int c) {
        if (tr[p].ch.find(c) != tr[p].ch.end()) {
            int q = tr[p].ch[c];
            if (tr[q].max == tr[p].max + 1) {
                endPos[q].insert(id);
                return q;
            }
            int nq = ++tot;
            endPos.push_back(emptyUnorderSet);
            tr.push_back(node(0, 0));
            endPos[nq].insert(id);//增加一个新节点，新节点的endPos集合就是他自己，由于我们并不需要知道endpos集合对于字符串
            // 的哪个位置，只需要知道它对应的哪个字符串就行，所以记录这里包含第几个字符串
            tr[nq] = tr[q];
            tr[nq].max = tr[p].max + 1;
            tr[q].fa = nq;
            while (p && (tr[p].ch.find(c) != tr[p].ch.end()) && tr[p].ch[c] == q) tr[p].ch[c] = nq, p = tr[p].fa;
            return nq;
        }
        int np = ++tot;
        endPos.push_back(emptyUnorderSet);
        tr.push_back(node(0, 0));
        tr[np] = node(tr[p].max + 1, 0);
        endPos[np].insert(id);
        while (p && (tr[p].ch.find(c) == tr[p].ch.end())) tr[p].ch[c] = np, p = tr[p].fa;
        if (!p) {
            tr[np].fa = rt;
        } else {
            int q = tr[p].ch[c];
            endPos[q].insert(id);
            if (tr[p].max + 1 == tr[q].max) {
                tr[np].fa = q;
            } else {
                int nq = ++tot;
                endPos.push_back(emptyUnorderSet);
                tr.push_back(node(0, 0));
                endPos[nq].insert(id);
                tr[nq] = tr[q];
                tr[nq].max = tr[p].max + 1;
                tr[np].fa = tr[q].fa = nq;
                while (p && (tr[p].ch.find(c) != tr[p].ch.end()) && tr[p].ch[c] == q) tr[p].ch[c] = nq, p = tr[p].fa;
            }
        }
        return np;
    }

    int n, k;
    std::vector<std::string> songName;

    inline void dfs(int x, int fa) {
        int v;
        for (int i = 0; i < q[x].size(); ++i) {
            v = q[x][i];
            if (v != fa) {
                dfs(v, x);
                if (endPos[x].size() < endPos[v].size()) {
                    endPos[x].swap(endPos[v]);
                }
                endPos[x].insert(endPos[v].begin(), endPos[v].end());
            }
        }
    }

    void buildSam() {
        //     freopen("sam.in", "r", stdin);
        // freopen("beifen2", "w", stdout);
        R(n);
        last = cur = tot = rt = 1;
        // endPos.resize(220005);
        endPos.push_back(emptyUnorderSet);
        endPos.push_back(emptyUnorderSet);//因为vector下标从0开始，所以一开始要push_back两次才能让编号同步
        tr.push_back(node(0, 0));
        tr.push_back(node(0, 0));
        songName.push_back("");
        songName.push_back("");
        for (int i = 1; i <= n; ++i) {
            last = rt;
            songName.push_back("");
            std::cin >> songName[i];
            int len = songName.size();
            id = i;
            for (int j = 0; j < len; ++j) {
                last = extend(last, songName[i][j]);
            }
        }
        q.resize(tot + 5);
        for (int i = 1; i <= tot; ++i) {
            q[tr[i].fa].push_back(i);
        }
        dfs(rt, rt);
        for (int i = 1; i <= tot; ++i) {
            printf("节点为%d ", i);
            for (std::tr1::unordered_set<int>::iterator iter = endPos[i].begin(); iter != endPos[i].end(); ++iter) {
                printf("%d ", *iter);
            }
            puts("");
        }
    }

    //打印相关的字符串
    void Print(int now) {
        int cnt = 0;
        puts("result\n");
        std::tr1::unordered_set<int> vis;
        while (cnt < 5) {
            if (now <= 1) break;
            for (std::tr1::unordered_set<int>::iterator iter = endPos[now].begin(); iter != endPos[now].end(); ++iter) {
                if (vis.find(*iter) == vis.end()) {
                    vis.insert(*iter);
                    std::cout << songName[*iter] << '\n';
                    ++cnt;
                    if (cnt >= 5) break;
                }
            }
            now = tr[now].fa;
        }
        puts("");
    }

    void findSam() {
        std::string matchString;
        std::cin >> matchString;
        int len = matchString.size();
        int now = rt;
        int c;
        for (int i = 0; i < len; ++i) {
            c = matchString[i];
            while (now && (tr[now].ch.find(c) == tr[now].ch.end())) now = tr[now].fa;
            //如果该节点没有c转移边，
            // 就跳后缀连接，直到空节点为止
            if (!now) now = rt;
            else now = tr[now].ch[c];
            //如果该节点为空节点，让他变成根节点
        }
        Print(now);
    }
} SAM1;