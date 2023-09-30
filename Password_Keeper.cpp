#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <list>
#include <iomanip>
#include <iterator>
#include <string>

typedef struct{
    char name[50];
    char password[50];
} Master;

std::string considerations(std::string app);

class Account{
private:
    char App_Site[50];
    char Acc_Name[50];
    char Acc_Password[50];
public:
    void takeInput(){
        std::string app, name, password;
        printf("    App/Website: "); std::getline(std::cin >> std::ws, app);
        printf("    Account Name: "); std::getline(std::cin >> std::ws, name);
        printf("    Password: "); std::getline(std::cin >> std::ws, password);

        app = considerations(app);

        std::strcpy(App_Site, app.c_str());
        std::strcpy(Acc_Name, name.c_str());
        std::strcpy(Acc_Password, password.c_str());
    }
    std::string return_App(){return App_Site;}
    std::string return_Name(){return Acc_Name;}
    std::string return_Password(){return Acc_Password;}
};

std::fstream rw;
std::list<Account> List_Of_Accounts;
Master M;

void typewriter(std::string word, float delay = 1.5);
void print(char x, size_t n);
std::string space(int n);
void pause(float n);
void erase_all();
void loading(float duration = 1.5, int space = 5, bool total_erase = false, bool should_pause = false);
void Set_Master();
bool is_Valid_Password(std::string word, bool &eight, bool &has_upper, bool &has_lower, bool &has_digit, bool &has_char, bool &no_space);
void Reset_Factory_Settings(bool &done);
bool Is_First_Time();
void Collect_Data_From_File();
bool present_In_List(std::string app, std::string name, std::list<Account> &L);
void Add_Account();
void Remove_Account();
void Search_Account();
char YN(std::string message, char first = 'y', char second = 'n');
bool Confirm_Master();
int display_Options(bool cls = true);
bool present_before(std::string app, std::list<std::string> &L);
std::string encrypt(std::string word);
std::string lowercase(std::string word);


int main(){
    /*bool done{false};
    Reset_Factory_Settings(done);*/

    if(Is_First_Time()) Set_Master();
    else{
        Collect_Data_From_File();
        bool is_Master{Confirm_Master()};
        if(is_Master){
            Collect_Data_From_File();
            bool done{false};
            int n{0};
            while(!done){
                Collect_Data_From_File();
                int choice{(n == 0) ? display_Options(false) : display_Options()};
                system("CLS");
                system("COLOR B0");
                if(choice == 1) Search_Account();
                else if(choice == 2) Add_Account();
                else if(choice == 3) Reset_Factory_Settings(done);
                else if(choice == 4){
                    system("CLS");
                    printf("\n   Thank you for using this program"); pause(1);
                    for(int i{0}; i < 3; i++){ printf(" ."); pause(1);} printf("\n");
                    return 0;
                }
                n++;
            }
        }
    }

}

void Collect_Data_From_File(){
    List_Of_Accounts.clear();
    rw.open("trial.bin", std::ios::in | std::ios::binary);
    int nth_time_Opened; size_t NoOfContents; Account A;
    rw.read(reinterpret_cast<char*>(&nth_time_Opened), sizeof(nth_time_Opened));
    rw.read(reinterpret_cast<char*>(&M), sizeof(M));
    rw.read(reinterpret_cast<char*>(&NoOfContents), sizeof(NoOfContents));

    for(size_t i{0}; i < NoOfContents; i++){
        rw.read(reinterpret_cast<char*>(&A), sizeof(A));
        List_Of_Accounts.push_back(A);
    }

    rw.close();
}

void Reset_Factory_Settings(bool &done){
    system("CLS");
    char yn_choice{YN("\n  Are you sure you want to delete all data saved?[y/n] : ")};
    if(yn_choice == 'y'){
        rw.open("trial.bin", std::ios::out | std::ios::binary);
        int i{0};
        rw.write(reinterpret_cast<char*>(&i), sizeof(i));
        rw.close();

        done = true;

        printf("\n   Goodbye, Master"); pause(1);
        for(int i{0}; i < 3; i++){ printf(" ."); pause(1);} printf("\n");

    }else{
        printf("Press ENTER to go back to the menu "); std::cin.ignore();
    }

}

bool Is_First_Time(){
    rw.open("trial.bin", std::ios::in | std::ios::binary);

    if(!rw.is_open()){
        rw.close();
        rw.open("trial.bin", std::ios::out | std::ios::binary);
        int i{0};
        rw.write(reinterpret_cast<char*>(&i), sizeof(i));
        rw.close();
    }

    int i;
    rw.read(reinterpret_cast<char*>(&i), sizeof(i));
    rw.close();

    if(i == 0) return true;
    else return false;
}

bool present_In_List(std::string app, std::string name, std::list<Account> &L){
    for(int i{0}; i < L.size(); i++){
        std::list<Account>::iterator indiv{L.begin()};
        std::advance(indiv, i);
        Account account{*indiv};

        if(account.return_App() == app && account.return_Name() == name) return true;
    }
    return false;
}

void Set_Master(){
    system("CLS");
    system("COLOR B0");
    printf("\n   WELCOME! "); pause(1.3);
    printf("\n\n   I am Fortrex, "); pause(1.3);
    printf("a Password Keeper Program."); pause(1.3); //erase_all();
    //printf("   here to help you in remembering your passwords.\n\n"); pause(1.3);
    //printf("Please input your details to become the master of this program."); pause(1.3);
    char yn_choice;
    std::string Master_Name, Master_Password;
    printf("\n\n\n");
    //int n{0};
    printf("   Tell me your name: "); std::getline(std::cin >> std::ws, Master_Name);
    system("CLS"); //printf("\n  Welcome, Maxine!"); pause(1.2);
    pause(0.5);

    typewriter("\n   Hello, "+Master_Name+"!", 1); pause(1.2);
    typewriter("\n\n   Now, you need to create a Master Password that will allow you to access your passwords.");
    pause(1.4); erase_all();


    int n{0};

    //printf("\n\n  Now, you need to create a Master Password that will allow you to access your passwords.");

    bool eight, has_upper, has_lower, has_digit, has_char, no_space;
    do{
        if(n == 0){
            typewriter("      Keep in mind that your password must have: \n");
            pause(0.3);
            printf("       %c atleast 8 characters\n", 'o'); pause(0.3);
            printf("       %c atleast one uppercase letter\n", 'o'); pause(0.3);
            printf("       %c atleast one lowercase letter\n", 'o'); pause(0.3);
            printf("       %c atleast one digit\n", 'o'); pause(0.3);
            printf("       %c atleast one special character\n", 'o'); pause(0.3);
            printf("       %c no space\n\n", 'o'); pause(1);
        }else{
            printf("      Your password has: \n");
            std::cout << "       " << (eight ? '/' : 'X') << " atleast 8 characters\n";
            std::cout << "       " << (has_upper ? '/' : 'X') << " atleast one uppercase letter\n";
            std::cout << "       " << (has_lower ? '/' : 'X') << " atleast one lowercase letter\n";
            std::cout << "       " << (has_digit ? '/' : 'X') << " atleast one digit\n";
            std::cout << "       " << (has_char ? '/' : 'X') << " atleast one special character\n";
            std::cout << "       " << (no_space ? '/' : 'X') << " no space\n\n";
        }
        printf("   Name: "); std::cout << Master_Name << "\n";
        printf("   Master Password: "); std::getline(std::cin >> std::ws, Master_Password);
        system("CLS");
        n++;

    } while(!is_Valid_Password(Master_Password, eight, has_upper, has_lower, has_digit, has_char, no_space));

    printf("\n     Processing...\n"); pause(0.5);

    loading(2.5,5); printf("\n");
    system("CLS");
    printf("\n     Processed!\n\n     Registration successful!"); pause(1.5); printf("\n     Your details have been saved."); pause(1.5);
    printf("\n     To complete your registration, "); pause(1.5);
    printf("you have to register atleast one account. "); pause(1.5);
    printf("\n\n     Press ENTER to proceed. "); std::cin.ignore();

    system("CLS");

    system("COLOR E0");

    bool continues{true};
    std::list<Account> Current_Accounts;
    do{
        printf("\n    ~ ~ ~ ~ ~ ~ Account Registration ~ ~ ~ ~ ~ ~\n\n");
        Account A;
        A.takeInput();
        if(present_In_List(A.return_App(), A.return_Name(), Current_Accounts)){
            printf("\n    This account name has been registered under this app/website before.\n");
            pause(1.5); printf("    Please try again."); pause(1.5);
            std::cout << "\r" << "    Press ENTER to proceed."; std::cin.ignore();
        }else{
            printf("\n");
            yn_choice = YN("    Are you sure you'd like to add this account?[y/n]: ");
            if(yn_choice == 'y') Current_Accounts.push_back(A);
            yn_choice = YN("    Would you like to add another[y/n]: ");
            if(yn_choice == 'n') continues = false;
        }
        system("CLS");
    } while(continues);

    size_t len{Current_Accounts.size()};

    if(len == 0){
        printf("\n    No account registered!"); pause(1);
        printf(" You cannot be my master."); pause(1);

        printf("\n    Escorting you out of this program ");
        for(int i{0}; i < 3; i++){ pause(1); printf(". ");}

    }else{
        rw.open("trial.bin", std::ios::out | std::ios::binary);
        int times{1};

        Master M;

        std::strcpy(M.name, Master_Name.c_str());
        std::strcpy(M.password, Master_Password.c_str());

        rw.write(reinterpret_cast<char*>(&times), sizeof(times));
        rw.write(reinterpret_cast<char*>(&M), sizeof(M));
        rw.write(reinterpret_cast<char*>(&len), sizeof(len));
        for(size_t i{0}; i < len; i++){
            std::list<Account>::iterator indiv{Current_Accounts.begin()};
            std::advance(indiv, i);
            Account account{(*indiv)};

            rw.write(reinterpret_cast<char*>(&account), sizeof(account));
        }
        rw.close();

        if(len == 1){
            std::list<Account>::iterator indiv{Current_Accounts.begin()};
            std::advance(indiv, 0);

            typewriter("\n    Adding account...\n", 0.4); pause(0.5);

            std::cout << "\n    App/Site: " << (*indiv).return_App();
            std::cout << "\n    Account Name: " << (*indiv).return_Name();
            std::cout << "\n    Account Password: " << (*indiv).return_Password();
        }else{
            printf("\n     ");
            std::cout << std::left << std::setw(5) << "     " << std::setw(30) << "APP/WEBSITE" << std::setw(30) << "USERNAME" << std::setw(30) << "PASSWORD" << std::endl;
            printf("    -----------------------------------------------------------------------------------\n");

            for(size_t i{0}; i < len; i++){
                std::list<Account>::iterator indiv{Current_Accounts.begin()};
                std::advance(indiv, i);
                printf("    ");
                std::cout << std::left << std::setw(5) << i+1 <<  std::setw(30) << (*indiv).return_App() << std::setw(30) << (*indiv).return_Name() << std::setw(30) << (*indiv).return_Password() << std::endl;
            }
        }

        printf("\n\n"); pause(1);
        loading(2, 10, true, true);
        std::cout << "\r          " << (len == 1 ? "Account" : "Accounts") << " added!";
        pause(1.5);
        printf("\r          Till we meet again, Master"); pause(1);
        for(int i{0}; i < 3; i++){ printf(". "); pause(1);} printf("\n");
    }
}

void typewriter(std::string word, float delay){
    size_t len{word.length()};
    for(size_t i{0}; i < len; i++){
        std::cout << word[i];
        pause(delay/len);
    }
}

void erase_all(){
    std::cout << "\r" << std::flush;
    for(int i{0}; i < 100; i++){
        printf(" ");
    }
    std::cout << "\r" << std::flush;
}

void pause(float n){
    n *= CLOCKS_PER_SEC;
    clock_t now{clock()};
    while(clock() - now < n);
}

void loading(float duration, int space, bool total_erase, bool should_pause){
    char bar[10]{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    float s{duration/10};
    printf("\n");
    for(int i{0}; i < 10; i++){
        bar[i] = 'o';
        for(int space_Ctr{0}; space_Ctr < space; space_Ctr++) std::cout << " ";
        printf("[ "); for(int j{0}; j < 10; j++) std::cout << bar[j] << " "; printf("]");
        pause(s);
        std::cout << "\r" << std::flush;
    }
    if(should_pause) pause(0.4);
    if(total_erase){
        for(int i{0}; i < 24 + space; i++)
            printf(" ");
        std::cout << "\r" << std::flush;
    }
}

bool is_Valid_Password(std::string word, bool &eight, bool &has_upper, bool &has_lower, bool &has_digit, bool &has_char, bool &no_space){
    //printf("\n");
    int uppercase{0}, lowercase{0}, number{0}, space{0}, characters{0};
    for(size_t i{0}; i < word.length(); i++){
        if(islower(word[i])) lowercase++;
        else if(isupper(word[i])) uppercase++;
        else if(isdigit(word[i])) number++;
        else if(word[i] == ' ') space++;
        else characters++;
    }
    eight = has_upper = has_lower = has_digit = has_char = no_space = true;

    if(word.length() < 8 || lowercase == 0 || uppercase == 0 || number == 0 || characters == 0 || space > 0){
        if(word.length() < 8){ eight = false;}
        if(lowercase == 0){ has_lower = false;}
        if(uppercase == 0){ has_upper = false;}
        if(number == 0){ has_digit = false;}
        if(characters == 0){ has_char = false;}
        if(space > 0){ no_space = false;}
    }

    if(word.length() < 8 || lowercase == 0 || uppercase == 0 || number == 0 || characters == 0 || space > 0){
        printf("\n   ");
        if(word.length() < 8){ printf("Your password must be atleast 8 characters long. Try again!");} //eight = false;}
        else if(lowercase == 0){ printf("There must be atleast one lowercase letter in your password. Try again!");}// has_lower = false;}
        else if(uppercase == 0){ printf("There must be atleast one capital letter in your password. Try again!");}// has_upper = false;}
        else if(number == 0){ printf("There must be atleast one number in your password. Try again!");}// has_digit = false;}
        else if(characters == 0){ printf("There must be atleast one character in your password. Try again!");}// has_char = false;}
        else if(space > 0){ printf("There must be no space in your password. Try again!");}// no_space = false;}
        printf("\n\n");
        return false;
    } else return true;
    printf("\n\n");
}

void Add_Account(){
    std::list<Account> Current_Accounts;
    bool continues{true};
    std::string app, name, password;
    char yn_choice;

    do{
        printf("\n    ~ ~ ~ ~ ~ ~ Account Registration ~ ~ ~ ~ ~ ~\n\n");
        Account A;
        A.takeInput();
        if(present_In_List(A.return_App(), A.return_Name(), Current_Accounts) || present_In_List(A.return_App(), A.return_Name(), List_Of_Accounts)){
            printf("\n    This account name has been registered under this app/website before.\n");
            pause(1.5);
        }else{
            printf("\n");
            yn_choice = YN("    Are you sure you'd like to add this account?[y/n]: ");
            if(yn_choice == 'y') Current_Accounts.push_back(A);
        }
        yn_choice = YN("    Would you like to add another[y/n]: ");
        if(yn_choice == 'n') continues = false;
        system("CLS");
    } while(continues);

    if(Current_Accounts.size() == 0){
        printf("\n   No account added."); pause(1);
        printf("\n   Press ENTER to proceed. "); std::cin.ignore();
    }else{
        int NoOfTimesOpened{1};

        List_Of_Accounts.insert(List_Of_Accounts.cend(), Current_Accounts.begin(), Current_Accounts.end());
        size_t len{List_Of_Accounts.size()};

        rw.open("trial.bin", std::ios::out | std::ios::binary);
        rw.write(reinterpret_cast<char*>(&NoOfTimesOpened), sizeof(NoOfTimesOpened));
        rw.write(reinterpret_cast<char*>(&M), sizeof(M));
        rw.write(reinterpret_cast<char*>(&len), sizeof(len));
        for(size_t i{0}; i < len; i++){
            std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
            std::advance(indiv, i);
            Account acc{(*indiv)};

            rw.write(reinterpret_cast<char*>(&acc), sizeof(acc));
        }

        rw.close();

        if(Current_Accounts.size() == 1){
            std::list<Account>::iterator indiv{Current_Accounts.begin()};
            std::advance(indiv, 0);

            printf("\n    Adding account . . .\n"); //pause(0.5);

            std::cout << "\n    App/Site: " << (*indiv).return_App();
            std::cout << "\n    Account Name: " << (*indiv).return_Name();
            std::cout << "\n    Account Password: " << (*indiv).return_Password();
        }else{
            printf("\n");
            std::cout << std::left << std::setw(5) << "   " << std::setw(30) << "APP/WEBSITE" << std::setw(30) << "USERNAME" << std::setw(30) << "PASSWORD" << std::endl;
            printf("  -----------------------------------------------------------------------------------\n");

            for(size_t i{0}; i < Current_Accounts.size(); i++){
                std::list<Account>::iterator indiv{Current_Accounts.begin()};
                std::advance(indiv, i);
                printf("  ");
                std::cout << std::left << std::setw(5) << i+1 <<  std::setw(30) << (*indiv).return_App() << std::setw(30) << (*indiv).return_Name() << std::setw(30) << (*indiv).return_Password() << std::endl;
            }
        }
        printf("\n\n"); pause(1);
        loading(2, 10, true, true);
        std::cout << "\r          " << (Current_Accounts.size() < 2 ? "Account" : "Accounts") << " added!";
        pause(1.5);
    }
}

bool present_before(std::string app, std::list<std::string> &L){
    for(size_t i{0}; i < L.size(); i++){
        std::list<std::string>::iterator indiv{L.begin()};
        std::advance(indiv, i);

        if((*indiv) == app) return true;
    }
    return false;
}

void Search_Account(){
    printf("\n\t              Input the details of the account you're searching for              ");
    printf("\n\t             -------------------------------------------------------              \n");

    bool App_Empty{false};
    bool found; std::string app, name;
    std::list<std::string> App;
    if(List_Of_Accounts.size() != 0){
        printf("\n  ~ Apps/Websites registered in the system ~\n\n");
        for(size_t i{0}; i < List_Of_Accounts.size(); i++){
            std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
            std::advance(indiv, i);
            std::string collected_app{(*indiv).return_App()};
            if(!present_before(collected_app, App)) App.push_back(collected_app);
        }

        size_t App_Len{App.size()};

        if(App_Len < 8){
            for(size_t j{0}; j < App.size(); j++){
                std::list<std::string>::iterator app_it{App.begin()};
                std::advance(app_it, j);
                std::cout << "    o " << (*app_it) << std::endl;
            }
        }else{
            for(size_t i{0}; i < App.size(); i++){
                std::list<std::string>::iterator firstRow{App.begin()}, secondRow{App.begin()};
                std::advance(firstRow, i);

                if(App_Len%2 == 0){
                    std::advance(secondRow, i+App_Len/2);
                    if(i<App_Len/2){
                        (*firstRow) += space(15 - (*firstRow).length()) + " o " + (*secondRow);
                        std::cout << "  o " << (*firstRow) << "\n";
                    }
                }else{
                    std::advance(secondRow, i+(App_Len/2)+1);
                    if(i<App_Len/2){
                        (*firstRow) += space(15 - (*firstRow).length()) + " o " + (*secondRow);
                        std::cout << "  o " << (*firstRow) << "\n";
                    } else if(i==App_Len/2) std::cout << "  o " << (*firstRow) << "\n";
                }
            }
        }

        printf("\n  App/Website: "); std::getline(std::cin >> std::ws,app);

        //{false};
        app = considerations(app);

        for(size_t i{0}; i < List_Of_Accounts.size(); i++){
            std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
            std::advance(indiv, i);
            Account acc{(*indiv)};

            if(acc.return_App() == app){found = true; break;}
            else found = false;
        }
    }else{
        printf("\n  ~ No app or website currently registered in the system ~\n");
        App_Empty = true;
    }

    if(found == false){
        if(!App_Empty) printf("\n     There is no account registered in this system linked to that app/website. Please try again!\n");
        pause(1);
        printf("\n\n     Press ENTER to proceed. ");
        std::cin.ignore(); //std::cin.ignore();
    }
    else{
        //\n  ~ Apps/Websites registered in the system ~\n\n
        //std::cout << "\n    Accounts linked to " << app << "\n";
        system("CLS");
        printf("\n\t              Input the details of the account you're searching for              ");
        printf("\n\t             -------------------------------------------------------              \n");
        std::cout << "\n  ~ Accounts linked to " << app << " ~\n\n";
        for(size_t i{0}; i < List_Of_Accounts.size(); i++){
            std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
            std::advance(indiv, i);
            Account acc{(*indiv)};

            if(acc.return_App() == app)
                std::cout << "    o " << acc.return_Name() << "\n";
        }
        std::string name, password;
        printf("\n  Account Name: "); std::getline(std::cin >> std::ws, name);

        system("CLS");
        printf("\n\t              Input the details of the account you're searching for              ");
        printf("\n\t             -------------------------------------------------------              \n");
        std::cout << "\n    App/Website: " << app;
        std::cout << "\n    Account Name: " << name;

        printf("\n\n     Searching for your account"); pause(0.5);
        for(int i{0}; i < 3; i++){ printf(" ."); pause(0.5);}

        if(present_In_List(app, name, List_Of_Accounts)){
            system("CLS");
            std::cout << "\n  Account found!\n\n"; //pause(1.2);
            //loading(1.5, 2, true, false);

            for(size_t i{0}; i < List_Of_Accounts.size(); i++){
                std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
                std::advance(indiv, i);
                Account acc{(*indiv)};

                if(acc.return_App() == app && acc.return_Name() == name) password = acc.return_Password();
            }

            size_t app_len{10+app.length()}, name_len{14 + name.length()}, pw_len{18 + password.length()};
            size_t greatest_len{(app_len >= name_len) ? ((app_len >= pw_len) ? app_len : pw_len) : ((name_len >= pw_len) ? name_len : pw_len)};
            greatest_len+=8;
            printf("  "); for(int i{0}; i < greatest_len; i++) printf("-");
            printf("\n  |"); print(' ', greatest_len-2); printf("|");
            printf("\n  |"); std::cout << "  App/Site: " << app; print(' ', greatest_len - app_len - 4); std::cout << "|";
            printf("\n  |"); std::cout << "  Account Name: " << name; print(' ', greatest_len - name_len - 4); std::cout << "|";
            printf("\n  |"); std::cout << "  Account Password: " << encrypt(password); print(' ', greatest_len - pw_len - 4); std::cout << "|";
            printf("\n  |"); print(' ', greatest_len-2); printf("|\n");
            printf("  "); for(int i{0}; i < greatest_len; i++) printf("-");

            char ab_choice; pause(1.2);
            printf("\n\n    [a] Reveal Password\n    [b] Remove Account\n");
            ab_choice = YN("\n    Choice: ", 'a', 'b');
            system("CLS");

            std::cout << "\n  Account found!\n\n";
            printf("  "); for(int i{0}; i < greatest_len; i++) printf("-");
            printf("\n  |"); print(' ', greatest_len-2); printf("|");
            printf("\n  |"); std::cout << "  App/Site: " << app; print(' ', greatest_len - app_len - 4); std::cout << "|";
            printf("\n  |"); std::cout << "  Account Name: " << name; print(' ', greatest_len - name_len - 4); std::cout << "|";


            if(ab_choice == 'a'){
                printf("\n  |"); std::cout << "  Account Password: " << password; print(' ', greatest_len - pw_len - 4); std::cout << "|";
                printf("\n  |"); print(' ', greatest_len-2); printf("|\n");
                printf("  "); for(int i{0}; i < greatest_len; i++) printf("-");
            }else if(ab_choice == 'b'){
                printf("\n  |"); std::cout << "  Account Password: " << encrypt(password); print(' ', greatest_len - pw_len - 4); std::cout << "|";
                printf("\n  |"); print(' ', greatest_len-2); printf("|\n");
                printf("  "); for(int i{0}; i < greatest_len; i++) printf("-");

                char yn_choice{YN("\n\n  Are you sure you want to remove this account from your database?[y/n]: ")};
                if(yn_choice == 'y'){
                    for(size_t i{0}; i < List_Of_Accounts.size(); i++){
                        std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
                        std::advance(indiv, i);

                        if((*indiv).return_App() == app && (*indiv).return_Name() == name) List_Of_Accounts.erase(indiv);
                    }

                    int NoOfTimesOpened{1}; size_t len{List_Of_Accounts.size()};

                    rw.open("trial.bin", std::ios::out | std::ios::binary);
                    rw.write(reinterpret_cast<char*>(&NoOfTimesOpened), sizeof(NoOfTimesOpened));
                    rw.write(reinterpret_cast<char*>(&M), sizeof(M));
                    rw.write(reinterpret_cast<char*>(&len), sizeof(len));

                    for(size_t i{0}; i < len; i++){
                        std::list<Account>::iterator indiv{List_Of_Accounts.begin()};
                        std::advance(indiv, i);
                        Account acc{(*indiv)};

                        rw.write(reinterpret_cast<char*>(&acc), sizeof(acc));
                    }
                    rw.close();
                }
                loading(1.5, 2, true, true);
                printf("  Account deleted!");
            }
        }else{
            printf("\r     Sorry, the account name wasn't found in the system.\n");
        }
        pause(1);
        printf("\n\n     Press ENTER to proceed. ");
        std::cin.ignore();
    }
}

std::string encrypt(std::string word){
    for(size_t i{0}; i < word.length()-3; i++)
        word[i] = '*';
    return word;
}

char YN(std::string message, char first, char second){
    std::string ans; bool continues{true};
    do{
        std::cout << message;
        std::getline(std::cin >> std::ws, ans);
        if(ans.length() == 1 && (ans[0] == first || ans[0] == second)){
            continues = false;
            return ans[0];
        }
        else{
            printf("  Invalid answer!"); pause(1);
            std::cout << "\r" << std::flush;
        }
    } while(continues);
    return 'w';
}

bool Confirm_Master(){
    system("CLS");
    system("COLOR 0F");
    char yn_choice;
    //printf("\n   STOP! "); pause(1.3);
    //printf("You've stumbled upon the guarded land of passwords."); pause(1.8);
    //typewriter("\n   STOP! ", 0.1); pause(1.3);

    typewriter("\n   Hello. ", 0.5); pause(0.3);
    //typewriter("You've stumbled upon the guarded land of passwords.", 0.1); pause(1.3);
    //erase_all();
    //typewriter("   I am Fortrex, ", 0.2); pause(1);
    //typewriter("a Password Keeper Program.", 0.2); pause(1);
    std::string name{M.name};
    printf("\n\n");
    std::string message{"   Are you ... " + name + "?[y/n] : "};
    yn_choice = YN(message);
    bool is_Master{false};

    if(yn_choice == 'n'){
        printf("\n\n   You are not my master."); pause(1);
        erase_all();
        std::cout << "\r" << std::flush;
        printf("   Escorting you out immediately");
        for(int i{0}; i < 3; i++){ printf(" ."); pause(1);} printf("\n");
    }
    else{
        int n{2};
        bool continues{true};
        do{
            std::string pw;
            printf("   Input master password: "); 
            std::getline(std::cin >> std::ws, pw);

            if(pw == M.password || n == 0){
                continues = false;
                if(pw == M.password) is_Master = true;
                else if(n == 0) is_Master = false;
            }
            else{
                if(n!=0) system("CLS");
                //printf("\n   Wrong Password! You have %d tries left.\n\n", n--);
                std::cout << "\n   Wrong Password! You have " << n << (n == 2 ? " tries" : " try") << " left.\n\n";
                std::cout << "   Name: " << M.name << "\n";
                n--;
            }
        }while(continues);

        loading(); system("CLS");

        if(!is_Master){
            printf("\n\n   You are NOT my master!"); pause(1.5);
            std::cout << "\r" << std::flush;
            printf("   Escorting you out of this program immediately");
            for(int i{0}; i < 3; i++){ printf(" ."); pause(1);} printf("\n");
        }else{
            system("COLOR E0");
            printf("\n  Welcome back, Master!"); pause(1.5);
        }
    }

    return is_Master;
}

int display_Options(bool cls){
    bool continues{true};
    std::string n;
    if(cls){
        system("CLS");
        system("COLOR E0");
    }
    do{
        printf("\n  What would you like to do today?\n\n  [1] Search for an account\n  [2] Add Account\n  [3] Reset Factory Settings\n  [4] End Program\n");
        printf("\n  Choice: "); std::getline(std::cin >> std::ws, n);

        if(n.length() == 1 && (n[0] == '1' || n[0] == '2' || n[0] == '3' || n[0] == '4')){
            return std::stod(n);
            continues = false;
        }else{
            printf("\n\tInvalid answer. Try again!"); pause(1);
            system("CLS");
        }
    }while(continues);
    return 10;
}

std::string considerations(std::string app){
    //fb- facebook, twt - twitter, ig - instagram
    app = lowercase(app);

    if(app == "fb") app = "facebook";
    else if(app == "twt") app = "twitter";
    else if(app == "ig") app = "instagram";

    app[0] = toupper(app[0]);
    return app;
}

std::string lowercase(std::string word){
    for(size_t i{0}; i < word.length(); i++){
        word[i] = tolower(word[i]);
    }
    return word;
}

void print(char x, size_t n){
    for(int i{0}; i < n; i++) std::cout << x;
}

std::string space(int n){
    std::string spaces;
    for(int i{0}; i < n; i++) spaces+= ' ';

    return spaces;
}
