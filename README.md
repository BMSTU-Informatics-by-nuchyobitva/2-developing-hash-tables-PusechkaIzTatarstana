# Отчет по ХЭШ-таблице "Герои", вариант 13(Открытая адресация: линейное пробирование)

## В данной прогрмамме реализована ХЭШ-таблица, содержащая данные о героях, у которых есть имя, уровень и класс(лучник, паладин, мечник и тд) 

### Открытая адресация: линейное пробирование

Это означает, что ХЭШ-таблица представляет собой обычный динамический массив, в котором коллизии разрешаются с помощью линейного пробирования, то есть попытки вставить элемент в следующую ячейку до тех пор, пока не окажется, что она пуста

### ХЭШ-функция

Я реализовал что-то похожее на MurmurHash3. В моей функции есть два блока - **миксер* и сама функция

1) миксер **mixBits**
```cpp
    unsigned int mixBits(unsigned int key) {// перемешивает биты в числе
        key ^= key >> 16;       
        key *= 0x85ebca6b;       
        key ^= key >> 13;         
        key *= 0xc2b2ae35;        
        key ^= key >> 16;        
        return key;
    }
```
Эта функция принимает на вход значение **key** и дальше с помощью побитовых сдвигов(>>), умножения(*=) и исключающего или(^=) **перемешивает* биты числа key
Значения 0x85ebca6b и 0xc2b2ae35 - специальные значения(они взяты из MurmurHash3), которые позволяют уменьшить количество коллизий

2) функция **hashFunction**
```cpp
    int hashFunction(string heroName, int tableSize) {     //Хэш-функция для строки
        unsigned int hash = 0; 
        for (char c : heroName) {
            
            hash = mixBits(hash ^ static_cast<unsigned int>(c));
        }
        
        hash = mixBits(hash ^ static_cast<unsigned int>(heroName.size()));
        
        return hash % tableSize;
    }
```
Эта функция принимает на вход ключ(имя персонажа) и размер таблицы, после чего к каждому отдельному char'у строки применяет **миксер*, перемешивая текущий хэш(изначально hash = 0) с хэшем char'а(c). После всех перемешиваний она также смешивает полученный хэш с размером строки. В итоге функция возвращает остаток от деления итогового хэша на размер таблицы, чтобы это число можно было использовать в качестве индекса 


## Алгоритм работы программы

В моей программе есть три основных блока кода:
* структура **Hero** - нужна для создания элементов таблицы
Содержит три параметра героя - имя, уровень и класс. В ней реализованы конструктор по умолчанию и пользовательский конструктор(для того, чтобы пользователь мог создать героя)

* класс **Hash_table** - нужен для реализации ХЭШ-таблицы и методов работы с ней
В нем реализованы вставка, поиск, удаление элемента, а также вывод всей таблицы. Также в нем содержатся вспомогательные фукнции и хэш-функция
  
* функция **main** - в ней описано взаимодействие с пользователем и выполнение соответствующих действий.
С помощью switch, cin и cout осуществляет связь между пользователем и таблицей героев

 ## Рассмотрим подробнее класс **Hash_table**
####private:
 
 * фунция **size**
```cpp
    int size(Hero* arr){
        int realsize = 0;
        for(int i = 0; i < capacity; i++){
            if(arr[i].name != "<empty>") realsize+=1;
        }
        return realsize;
    }
```
Данная функция считает количество непустых элементов(realsize), проверяя является ли имя обьекта <empty>(такое имя только у инициализированных по умолчанию)

* функция **increaser**
```cpp
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
    }
```
Эта функция нужна для увеличения массива в случае, если он будет заполнен более чем на 70%. У нее несложный алгоритм: задается новый размер для массива(new_capacity), который больше старого размера в 2 раза. После этого делается новый массив размера new_capacity, в него переносятся все элементы, старый массив удаляется, а переменной старого массива присваевается указатель на новый массив

* функция  **search_index**
```cpp
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
```
Эта функция принимает ключ(nm) и возвращает либо идекс массива, в котором содержится герой с таким именем, либо -1(это значит что элемента с таким именем нет. Флажок t нужен для того, чтобы в худшем случае перебрать все элементы в массиве, так как поиск осуществляется(из-за выбранного способа борьбы с коллизиями), начиная с элемента a(хэш nm = a)

####public:

* конструктор и деструктор Hash_table
Конструктор инициализирует массив размером 7 элементов, деструктор удаляет массив для освобождения места в памяти

* функция **insert**
```cpp
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
```
Эта функция нужна для добавления элемента в массив. Сначала она проверяет, есть ли уже элемент с таким именем, и в случае нахождения просит добавлять героя с другим именем. Далее применяется **increaser**, чтобы в случае надобности увеличить массив. После проверки размера происходит непосредственно добавление элемента с помощью хэш-функции, соответствующее принципу линейного пробирования

* функция **search**
```cpp
    void search(string nm){
        bool t = false;
        int a = hashFunction(nm, capacity);
        
        if(arr[a].name == nm){
            cout << "Вот что мы нашли по ключу " << nm << ": " << "класс персонажа - " << arr[a].charaterClass << ", уровень - " << arr[a].lvl << endl;

        }
        else{
            for(int i = a + 1; i < capacity; i++){
                if(arr[i].name == nm){
                    cout << "Вот что мы нашли по ключу " << nm << ": " << "класс персонажа - " << arr[i].charaterClass << ", уровень - " << arr[i].lvl<< endl;

                }   
            }
            
            for(int i = 0; i < a; i++){
                if(arr[i].name == nm){
                    cout << "Вот что мы нашли по ключу " << nm << ": " << "класс персонажа - " << arr[i].charaterClass << ", уровень - " << arr[i].lvl<< endl;

                }   
            }
        }
        cout << "Нам не удалось найти персонажа с таким именем(" << endl;
    }
```
Эта функция работает аналогично функции **search_index**, но вместо возвращения индекса выводит найденные данные, либо сообщение о том, что такого элемента в таблице нет

* функция **deleted**
```cpp
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
```
Эта функция нужна для удаления элемента. В ней к имени героя применяется **search_index** и если элемент найден, то все его параметры приводятся к значениям по умолчанию. В противном случае выводится сообщение о том, что такого элемента нет

* функция **print**
```cpp
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
```
Эта функция выводит всю табличку, перебирая элементы массива. Переменная vived_el нужна для того, чтобы после вывода всех элементов не продолжать проход по массиву
![image](https://github.com/user-attachments/assets/66e8828e-aa83-4180-9cc7-dc884ad80ee4)






























