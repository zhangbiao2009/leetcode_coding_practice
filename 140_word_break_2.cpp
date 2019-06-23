class Solution {
    struct TreeNode{
        string word;
        bool isWordEnd;
        unordered_map<char, TreeNode*> children;
        TreeNode() : isWordEnd(false) {}
    };
    void build(TreeNode* root, const string& s){
        auto np = root;
        for(int i=0; i<s.length(); i++){
            if(np->children[s[i]] == NULL){
                np->children[s[i]] = new TreeNode;
            }
            np = np->children[s[i]];
        }
        np->isWordEnd = true;
        np->word = s;
    }
    TreeNode* buildTrie(const vector<string>& words){
        TreeNode* root = new TreeNode;
        for(int i=0; i<words.size(); i++){
            build(root, words[i]);
        }
        return root;
    }
    
    struct WordNode{
        string currWord;    // currWord为空代表是个merge node；
        list<WordNode*> prevNodes;
        WordNode() {}
        WordNode(const string& w) : currWord(w) {}
    };
    
    void getResult(WordNode* t, vector<string>& curr, vector<string>& res){
        // assert t != NULL
        if(!t->currWord.empty())
            curr.push_back(t->currWord);
        for(auto it = t->prevNodes.begin(); it != t->prevNodes.end(); it++)
            getResult(*it, curr, res);
        if(t->prevNodes.empty()){   // 没有子节点，是个叶子节点
            if(!curr.empty()){
                // join
                string s;
                bool first = true;
                for(int i=(int)curr.size()-1; i>=0; i--){
                    if(!first)
                        s += " ";
                    s += curr[i];
                    first = false;
                }
                res.push_back(s);
            }
        }
        if(!t->currWord.empty())
            curr.pop_back();
    }
    
public:
    
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        TreeNode* root = buildTrie(wordDict);
        unordered_map<TreeNode*, WordNode*> states; // state node => word tree
        states[root] = NULL;    // 初始化word tree是空
        for(int i=0; i<s.length(); i++){
            unordered_map<TreeNode*, WordNode*> tmp;
            for(auto it=states.begin(); it!=states.end(); it++){
                auto np = it->first;
                auto mit = np->children.find(s[i]);
                if(mit == np->children.end()){
                    it->second = NULL;     // 无望到达终点，清除上面的word tree
                    continue;
                }
                // 插入下一个状态，同时把word tree转移过来
                auto wordTree = it->second;
                it->second = NULL;
                tmp[mit->second] = wordTree;
                if(mit->second->isWordEnd){
                    // 构造一个append了当前word的tree，合并到root中去
                    auto newTree = new WordNode(mit->second->word);
                    if(wordTree != NULL)
                        newTree->prevNodes.insert(newTree->prevNodes.end(), wordTree);
                    if(tmp[root] == NULL){
                        tmp[root] = new WordNode;
                    }
                    tmp[root]->prevNodes.insert(tmp[root]->prevNodes.end(), newTree);
                }
            }
            swap(states, tmp);
        }
        vector<string> curr;
        vector<string> res;
        if(states[root] == NULL)
            return res;
        getResult(states[root], curr, res);
        return res;
    }
};
