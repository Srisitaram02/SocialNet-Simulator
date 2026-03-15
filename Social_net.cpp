#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cctype>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std ;

string lowercase(string username) {
    for (int i = 0; i < username.size(); i++) {
        username[i] = tolower(username[i]);
    }
    return username;
}

class Node {
    public:
    string username;
    time_t timestamp;
    string content;
    Node* left;
    Node* right;
    int height;
    Node( string usernm , time_t ts, string c ){
        username = usernm;
        timestamp = ts;
        content = c;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

class AVLtree{

    public: 

    Node* root;

    void delete_tree(Node* root) {
        if (root != nullptr) {
            delete_tree(root->left);
            delete_tree(root->right);
            delete root;
        }
    }

    AVLtree(){
        root = nullptr;
    }

    ~AVLtree(){
        delete_tree(root);
    }

    int get_height( Node* n ) { 
        if( n == nullptr ){
            return 0;
        }
        return n -> height;
    }
    void update_height( Node* n ) { 
        if (n == nullptr){
            return;
        } 
        n->height = 1 + max( get_height( n->left ), get_height (n->right ) ); 
    }
    int balance( Node* n ) { 
        if( n == nullptr ){
            return 0;
        }
        return get_height(n->left) - get_height(n->right);
    }

    void print_inorder( vector<string> &v , Node* root ){
        if( root == nullptr ){
            return;
        }
        print_inorder( v , root -> left );
        bool exists = false;
        for (auto name : v) {
            if (name == root->username) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            v.push_back(root->username);
        }
        print_inorder( v , root -> right );
    }

    Node* rotate_Right( Node* root ){
        if (!root || !(root->left)) return root;
        Node* child = root -> left;
        Node* rightchild = child -> right;

        child -> right = root;
        root -> left = rightchild;

        update_height( root );
        update_height( child );

        return child;
    }

    Node* rotate_Left( Node* root ){
        if (!root || !(root->right)) return root;
        Node* child = root -> right;
        Node* leftchild = child -> left;

        child -> left = root;
        root -> right = leftchild;

        update_height( root );
        update_height( child );

        return child;
    }

    Node* add_Friend_Helper(Node* root, string username) {
        if (!root) return new Node(username, 0, "");
        if (username < root->username)
            root->left = add_Friend_Helper(root->left, username);
        else if (username > root->username)
            root->right = add_Friend_Helper(root->right, username);
        else
            return root;
    
        update_height(root);
        int bal = balance(root);
    
        if (bal > 1 && username < root->left->username)
            return rotate_Right(root);
        if (bal < -1 && username > root->right->username)
            return rotate_Left(root);
        if (bal > 1 && username > root->left->username) {
            root->left = rotate_Left(root->left);
            return rotate_Right(root);
        }
        if (bal < -1 && username < root->right->username) {
            root->right = rotate_Right(root->right);
            return rotate_Left(root);
        }
    
        return root;
    }

    Node* helper_add( Node* root , string usernm , string content , time_t nowtime){
        if( root == nullptr ){
            return new Node(usernm , nowtime , content);
        }
        if( root -> timestamp > nowtime ){
            root -> left = helper_add( root-> left, usernm , content , nowtime );
        }
        else if( root -> timestamp < nowtime ){
            root -> right = helper_add( root-> right , usernm , content , nowtime );
        }
        else {
            if (usernm < root->username)
                root->left = helper_add(root->left, usernm, content, nowtime);
            else if (usernm > root->username)
                root->right = helper_add(root->right, usernm, content, nowtime);
            else
                return root;
        }
        update_height( root );
        int bal = balance( root );

        if( bal > 1 && nowtime < root -> left -> timestamp ){
            return rotate_Right( root );
        }
        if( bal < -1 && nowtime > root -> right -> timestamp ){
            return rotate_Left( root );
        }
        if( bal > 1 && nowtime > root -> left -> timestamp ){
            root -> left = rotate_Left( root );
            return rotate_Right( root );
        }
        if( bal < -1 && nowtime < root -> right -> timestamp ){
            root -> right = rotate_Right( root );
            return rotate_Left( root );
        }

        return root;
    }

    void add( string username , string content ){
        root = helper_add( root , username , content , time(0));
    }

    Node* get_root(){
        return root;
    }
};

class Graph{

    public:

    unordered_map< string , AVLtree > mp;
    unordered_map< string , AVLtree > post;

    vector<string> Give_Friends( string username ){
        if (mp.find(username) == mp.end()) {
            vector<string> st;
            return st;
        }
        AVLtree &used = mp[username];
        vector<string> v;
        used.print_inorder( v, used.get_root());
        return v;
    }

    void inorder_collect(Node* root, vector<pair<time_t, string> >& v) {
        if (!root) return;
        inorder_collect(root->left, v);
        v.push_back(make_pair(root->timestamp, root->content));
        inorder_collect(root->right, v);
    }

    void ADD_USER( string username ){
        if( mp.find(username) != mp.end()){
            cout << "User " << username << " already exists." << endl;
            return;
        }
        AVLtree t;
        mp[username] = t;
        AVLtree v;
        post[username] = v;
        cout << "User " << username << " succesfully created." << endl;
    }

    void ADD_FRIEND( string username1, string username2 ){
        if( mp.find(username1) == mp.end()){
            cout << "No user with " << username1 << " exists . Enter a valid user to make friends with." << endl;
            return;
        }
        if( mp.find(username2) == mp.end()){
            cout << "No user with " << username2 << " exists . Enter a valid user to make friends with." << endl;
            return;
        }
        if (username1 == username2) {
            cout << "A user cannot be friends with themselves." << endl;
            return;
        }
        vector<string> f1 = Give_Friends(username1);
        bool alreadyFriend = false;
        for (auto fr : f1) {
            if (fr == username2) {
                alreadyFriend = true;
                break;
            }
        }
        if (alreadyFriend) {
            cout << username1 << " is already a friend of " << username2 << endl;
            return;
        }

        AVLtree &used1 = mp[username1];
        AVLtree &used2 = mp[username2];
        used1.root = used1.add_Friend_Helper(used1.root, lowercase(username2));
        used2.root = used2.add_Friend_Helper(used2.root, lowercase(username1));
        cout << "Users " << username1 << " and " << username2 << " are now friends." << endl;
    }

    void LIST_FRIENDS( string username ){
        if (mp.find(username) == mp.end()) {
            cout << "User not found. Please give a valid user." << endl;
            return;
        }
        AVLtree &used = mp[username];
        vector<string> v;
        used.print_inorder( v, used.get_root());
        if (v.empty()) {
            cout << "No friends yet." << endl;
            return;
        }
        for (auto name : v) {
            cout << name << endl;
        }
    }

    static bool compare_Pairs( pair<string, int> a , pair<string, int> b){
        if (a.second != b.second){
            return a.second > b.second;
        }
        else
            return a.first < b.first;
    };

    void SUGGEST_FRIENDS( string username , int N ){
        unordered_map<string, int> rank;
        vector<string> friends;
        friends = Give_Friends( username );
        if( N == 0 ) return;
        if( N < 0 ) {
            cout << " N must be non-negative." << endl;
            return;
        }
        for( auto f1 : friends ){
            vector<string> temp;
            temp = Give_Friends( f1 ); 
            for( auto f2 : temp ){
                bool isFriend = false;
                for (auto x : friends) {
                    if (x == f2) { 
                        isFriend = true; 
                        break; 
                    }
                }
                if ( f2 != username && !isFriend ) {
                    rank[f2]++;
                }
            }
        }
        vector<pair<string, int> > v( rank.begin() , rank.end() );

        sort(v.begin(), v.end(), Graph::compare_Pairs);

        if (v.empty()) {
            cout << "No suggestions available." << endl;
            return;
        }

        if( N > v.size()){
            for (int i = 0; i < v.size(); i++) {
                cout << v[i].first << endl;
            }
        }
        else{
            for (int i = 0; i < N; i++) {
                cout << v[i].first << endl;
            }
        }
    }

    void DEGREES_OF_SEPARATION( string username1 , string username2 ){
        if (mp.find(username1) == mp.end() || mp.find(username2) == mp.end()) {
            cout << "User not found." << endl;
            return;
        }
        queue<string> q;
        unordered_map<string, int> dist;
        dist[username1] = 0;
        q.push( username1 );
        while( !q.empty()){
            string curr = q.front();
            q.pop();

            if (curr == username2) {
                cout << dist[curr] << endl;
                return;
            }

            vector<string> friends = Give_Friends(curr);
            for (auto friendName : friends) {
                if (dist.find(friendName) == dist.end()) {
                    dist[friendName] = dist[curr] + 1;
                    q.push(friendName);
                }
            }
        }
        cout << "-1" << endl;
    }

    void ADD_POST( string username , string content ){
        if (post.find(username) == post.end()) {
            cout << "User not found. Please give a valid user." << endl;
            return;
        }
        AVLtree &now = post[username];
        now.add( username , content );
        cout << "Post succesfully added for '" << username << "' ." << endl;
    }

    void OUTPUT_POSTS(string username, int N) {
        if (post.find(username) == post.end()) {
            cout << "User not found." << endl;
            return;
        }
        if (N < -1) {
            cout << " N >= -1 is must." << endl;
            return;
        }
        vector<pair<time_t, string> > posts;
        inorder_collect(post[username].get_root(), posts);
        if (posts.empty()) {
            cout << "No posts yet." << endl;
            return;
        }
        reverse(posts.begin(), posts.end());
        if (N == -1 || N > (int)posts.size()) N = posts.size();
        for (int i = 0; i < N; i++) {
            cout << posts[i].second << endl;
        }
    }
};

int main(){

    Graph gh;
    while(true){
        string input , operation , rest , username , leftout , extra;
        cout << endl;
        cout << "Enter the command you want to perform or EXIT if you want to end the programme. " << endl;
        cout << endl;

        getline(cin, input);
        if (input.empty()) continue;
        stringstream ss(input);

        ss >> operation;
        if ( !( operation == "ADD_USER" || operation == "ADD_FRIEND" || operation == "LIST_FRIENDS" || operation == "SUGGEST_FRIENDS" || operation == "DEGREES_OF_SEPARATION" || operation == "ADD_POST" || operation == "OUTPUT_POSTS" || operation == "EXIT" ) ){
            cout << "Incorrect operation given, please choose : ADD_USER , ADD_FRIEND , LIST_FRIENDS , SUGGEST_FRIENDS , DEGREES_OF_SEPARATION , ADD_POST , OUTPUT_POSTS , EXIT " << endl;
            continue;
        }

        if( operation == "EXIT" ){
            if((ss >> rest)){
                cout << "After EXIT, something extra have been written which is a wrong format. If you want to end, write only : EXIT " << endl;
                continue;
            }
            break;
        }

        else if( operation == "ADD_USER"){
            if( !(ss >> rest) ){
                cout << "Incorrect format, please write it in ADD_USER <username> format. ";
                continue;
            }
            if( ss >> leftout ){
                cout << "Incorrect format, after username , something extra has been written. Please write it in ADD_USER <username> format. ";
                continue;
            }
            username = lowercase( rest );

            gh.ADD_USER( username );
            continue;
        }

        else if( operation == "ADD_FRIEND"){
            if( !( ss>> rest )){
                cout << "Incorrect format, please write it in ADD_FRIEND <username1> <username2> format. ";
                continue;
            }
            if( !( ss >> leftout )){
                cout << "Incorrect format, please write it in ADD_FRIEND <username1> <username2> format. ";
                continue;
            }
            if( ss >> extra ){
                cout << "Something extra have been written after username2, username cannot have spaces , please write it in ADD_FRIEND <username1> <username2> format. ";
                continue;
            }
            string username1 = lowercase(rest);
            string username2 = lowercase(leftout);

            gh.ADD_FRIEND( username1, username2 );
            continue;
        }

        else if( operation == "LIST_FRIENDS"){
            if( !(ss >> rest) ){
                cout << "Incorrect format, please write it in LIST_FRIENDS <username> format. ";
                continue;
            }
            if( ss >> leftout ){
                cout << "Incorrect format, after username , something extra has been written. Please write it in LIST_FRIENDS <username> format. ";
                continue;
            }
            username = lowercase( rest );

            gh.LIST_FRIENDS( username );
            continue;
        }

        else if( operation == "SUGGEST_FRIENDS" ){
            if( !(ss >> rest) ){
                cout << "Incorrect format, please write it in SUGGEST_FRIENDS <username> <N> format. ";
                continue;
            }
            if( !( ss>> leftout)){
                cout << "Incorrect format, please write it in SUGGEST_FRIENDS <username> <N> format. ";
                continue;
            }

            if( ss >> extra ){
                cout << "Something extra have been written after N, please write it in SUGGEST_FRIENDS <username> <N> format. ";
                continue;
            }

            try{
                size_t pos;
                int num = stoi( leftout , &pos );
                if( pos != leftout.size()){
                    cout << "Error: invalid characters after number" << endl;
                    continue;
                }
                string username = lowercase( rest );
                gh.SUGGEST_FRIENDS( username , num );
            }
            catch (...){
                cout << "Error: Please provide a valid number N in SUGGEST_FRIENDS <username> <N> format." << endl;
                continue;
            }
        }

        else if( operation == "DEGREES_OF_SEPARATION" ){
            if( !( ss >> rest )){
                cout << "Incorrect format, please write it in DEGREES_OF_SEPARATION <username1> <username2> format. ";
                continue;
            }
            if( !( ss >> leftout )){
                cout << "Incorrect format, please write it in DEGREES_OF_SEPARATION <username1> <username2> format. ";
                continue;
            }
            if( ss >> extra ){
                cout << "Something extra have been written after username2, username cannot have spaces , please write it in DEGREES_OF_SEPARATION <username1> <username2> format. ";
                continue;
            }
            string username1 = lowercase( rest );
            string username2 = lowercase( leftout );
            gh.DEGREES_OF_SEPARATION( username1 , username2 );
            continue;
        }

        else if( operation == "ADD_POST"){
            if( !( ss >> rest )){
                cout << "Incorrect format, please write it in ADD_POST <username> <post content> format. ";
                continue;
            }
            
            string username1 = lowercase( rest );

            string postContent;
            string g;
            while(ss>>g) postContent += g+" ";

            if (postContent.empty()) {
                cout << "Error: missing post content." << endl;
                continue;
            }
            postContent = lowercase(postContent);

            gh.ADD_POST(username1, postContent);
            continue;
        }

        else if (operation == "OUTPUT_POSTS") {
            if( !(ss >> rest )){
                cout << "Incorrect format, please write it in OUTPUT_POSTS <username> <N> format. ";
                continue;
            }
            if( !( ss >> leftout )){
                cout << "Incorrect format, please write it in OUTPUT_POSTS <username> <N> format. ";
                continue;
            }
            if( ss >> extra ){
                cout << "Something extra have been written after N, please write it in OUTPUT_POSTS <username> <N> format. ";
                continue;
            }
            try{
                size_t pos;
                int num = stoi( leftout , &pos );
                if( pos != leftout.size()){
                    cout << "Error: invalid characters after number" << endl;
                    continue;
                }
                string username = lowercase( rest );
                gh.OUTPUT_POSTS( username , num );
            }
            catch (...){
                cout << "Error: Please provide a valid number N in OUTPUT_POSTS <username> <N> format." << endl;
                continue;
            }
        }
    }
    return 0;
}