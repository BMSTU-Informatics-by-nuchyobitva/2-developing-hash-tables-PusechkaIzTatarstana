```cpp
#include <iostream>
#include <string>

using namespace std;

struct Hero{ 
    int lvl;
    string name; //ключ
    string charaterClass;
    Hero(){
        lvl = 0;
        name = "<empty>"; 
        charaterClass = "<empty>";
    }; //конструктор структуры
    Hero(int a, string b, string c){
        lvl = a;
        name = b; 
        charaterClass = c;
    }
};


class Hash_table{
private:
    Hero* arr;
    size_t capacity;
    int realsize; //кол-во элементов(значащих) в таблице
    
    int size(Hero* arr){
        int realsize = 0;
        for(int i = 0; i < capacity; i++){
            if(arr[i].name != "<empty>") realsize+=1;
        }
        return realsize;
    }
        unsigned int mixBits(unsigned int key) {// перемешивает биты в числе
            key ^= key >> 16;       
            key *= 0x85ebca6b;       
            key ^= key >> 13;         
            key *= 0xc2b2ae35;        
            key ^= key >> 16;        
            return key;
        }
    

    int hashFunction(const std::string& heroName, int tableSize) {     //Хэш-функция для строки
        unsigned int hash = 0;  

        for (char c : heroName) {
            
            hash = mixBits(hash ^ static_cast<unsigned int>(c));
        }
        
        hash = mixBits(hash ^ static_cast<unsigned int>(heroName.size()));
        
        return hash % tableSize;
    }
    
     
    void increaser() { // функция увеличивает массив, если это требуется
    
        if(double(realsize + 1)/double(capacity) > 0.7){
            size_t new_capacity = capacity*2;
            Hero* arr1 = new Hero[new_capacity];
            for(int i = 0; i < capacity; i++){
                if(arr[i].name != "<empty>"){
                    int a = hashFunction(arr[i].name, new_capacity);
                    arr1[a] = arr[i];
                }
            }
            delete [] arr;
            arr = arr1; 
            capacity = new_capacity;
        }
    } // функция увеличивает массив, если это требуется
    
    int search_index(string nm){ //костыльная функция
        bool t = false;
        int a = hashFunction(nm, capacity);
        if(arr[a].name == nm){
            return a;
        }
        else{
            for(int i = a + 1; i < capacity; i++){
                if(arr[i].name == nm){
                    return i;
                    t = true;
                }   
            }
        }
        
        if(t == false){
            for(int i = 0; i < a; i++){
                if(arr[i].name == nm) {
                    return i;
                    t = true;
                }   
            }
            if(t == false) return -1;
        }
        return -1;
    }
    
    
public:
    Hash_table() {
        capacity = 7;
        arr = new Hero[capacity];
        realsize = 0;
    }
    
    ~Hash_table() {
        delete[] arr;
    }

    void insert(Hero hero){
        if (search_index(hero.name) == -1){
            
            bool t = false;
            increaser();
            int a = hashFunction(hero.name, capacity);
            if(arr[a].name == "<empty>"){
                arr[a] = hero;
            }
            else{
                for(int i = a + 1; i < capacity; i++){
                    if(arr[i].name == "<empty>"){
                        arr[i] = hero;
                        t = true;
                        break;
                    }   
                }
                if(t == false){
                    for(int i = 0; i < a; i++){
                        if(arr[i].name == "<empty>"){
                            arr[i] = hero;
                            t = true;
                            break;
                        }   
                    }
                }
            }
            realsize += 1;
        }
        else cout << "Такое имя уже есть! Попробуй другое!" << endl;
    }
    
    
    void search(string nm){
        bool t = false;
        int a = hashFunction(nm, capacity);
        
        if(arr[a].name == nm){
            cout << "Вот что мы нашли по ключу " << nm << ": " << "класс персонажа - " << arr[a].charaterClass << ", уровень - " << arr[a].lvl << endl;
            return;
        }
        else{
            for(int i = a + 1; i < capacity; i++){
                if(arr[i].name == nm){
                    cout << "Вот что мы нашли по ключу " << nm << ": " << "класс персонажа - " << arr[i].charaterClass << ", уровень - " << arr[i].lvl<< endl;
                    return;
                }   
            }
            
            for(int i = 0; i < a; i++){
                if(arr[i].name == nm){
                    cout << "Вот что мы нашли по ключу " << nm << ": " << "класс персонажа - " << arr[i].charaterClass << ", уровень - " << arr[i].lvl<< endl;
                    return;
                }   
            }
        }
        cout << "Нам не удалось найти персонажа с таким именем(" << endl;
    }
    
    void deleted(string nm){
        int index = search_index(nm);
        if (index != -1){
            arr[index].name = "<empty>";
            arr[index].lvl = 0;
            arr[index].charaterClass = "<empty>";
            realsize -= 1;
        }
        else{
            cout << "Вы пытаетесь удалить персонажа, которого не существует!" << endl;
        }
    }
    
    void print(){
        if(realsize != 0){
            int vived_el = 0; //кол-во выведенных элементов
            for(int i = 0; i < capacity; i++){
                if(arr[i].name != "<empty>") {
                    cout << endl << "/////" << endl << "Имя: " << arr[i].name << endl << "Класс: " << arr[i].charaterClass << endl << "Уровень: " << arr[i].lvl << endl << "/////" << endl;
                    vived_el += 1;
                    if(vived_el >= realsize) break;
                }
            }
        }
        else cout << "Нечего выводить!" << endl;
    }
};
    

int main()
{
    Hash_table table;
    int c;
    do {
        cout << "////" << endl <<"Что вы хотите? " << endl << "1 - вставить элемент" << endl << "2 - удалить элемент" << endl << "3 - вывести ХЭШ-табличку" << endl << "4 - поиск элемента" << endl << "-1 - выход" << endl << "////" << endl;
        cin >> c;
        
        switch(c){
            
            case 1:
            {
                string nm;
                int level;
                string classa;
                cout << "Введите имя: ";
                cin >> nm;
                cout << "Введите уровень(по умолчанию 0): ";
                cin >> level;
                cout << "Введите класс героя: ";
                cin >> classa;
                Hero aboba(level, nm, classa);
                table.insert(aboba);
                break;
            }
            case 2:
            {
                string nm;
                cout << "Введите имя: ";
                cin >> nm;
                table.deleted(nm);
                break;
            }
            case 3:
            {
                table.print();
                break;
            }
            case 4:
            {
                string nm;
                cout << "Введите имя для поиска: ";
                cin >> nm;
                table.search(nm);
                break;
            }
            case -1:
                break;
            default:
                cout << "Неверный выбор!" << endl;
        }
```
        
    } while(c != -1);

    return 0;
}
