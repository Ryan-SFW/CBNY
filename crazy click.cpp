#include <graphenelib/contract.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/print.hpp>
#include <graphenelib/types.h>
#include <vector>
#include <math.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define e 2.718282
#define ffa 
#define t0 0.02

using namespace graphene;

class users : public contract
{
    private:
    std::string usersname;
    std::string account;
    std::vector<float> timepoints;
    std::vector<float>::iterator it; 
    float P;
    float k1, k2, k3, k4, K1, K2, K3, K4;
    float n1, n2, n3, n4;
    float fjnow， P;
    float* ptr_fjnow=&fjnow, ptr_P=&P;
    int wintime=0;
    float rate;
    
    public:
    users(std::string Usersname){
        usersname=Usersname;
        vector<float> timepoints;
    }
    ~users(){};
    std::string get_username();
    void set_username();
    void set_wintime(){
        wintime++;
    };    
    float nowheight();
    float nowtime();
  
    void settime(){
        time_t timepoint;
        time(&timepoint);
        timepoints.push_back(time_t timepoint);
    };
    
    float F1(){
        time_t t1, t2, t3;
        double cha1, cha12;
        t1=timepoints[-1];
        t2=timepoints[-2];
        t3=timepoints[-3];
        cha1=difftime(t3,t2);
        cha2=difftime(t2,t1);
        return ((1*cha1+2*cha2)-(cha1+cha2))/2;
    };
    
    float f1(float* ptr_fjnow, float F1()){
        *ptr_fjnow=(n1*pow(e,k1*(F1()-P))-n2*pow(e,-k2*fjnow))*t0+fjnow;
        return fjnow;
    };
    
    float f2(float* ptr_P, float* ptr_fjnow, float F1()){
        *ptr_P=n3*pow(e,k3*(*ptr_fjnow-ffa))*t0*(*ptr_P) + *ptr_P;
        return *ptr_P;
    };
    
    float f3(float* ptr_P, float F1()){
        *ptr_P = (F1()-*ptr_P)*0.13*t0 + *ptr_P;
        return *ptr_P;
    };
    
    // K1<<K2
    float f4(float* ptr_P){
        *ptr_P = K1*t0 + *ptr_P;
        return *ptr_P;
    };
    
    float f5(float* ptr_P){
        *ptr_P = K2*t0 + *ptr_P;
        return *ptr_P;
    };
    
    float f6(float* ptr_P, float F1()){
        *ptr_P = -n4*pow(e,K4*(2-*ptr_P+ Pmo))*t0 + *ptr_P;
        return *ptr_P;
    };
    
    float f7(float* ptr_P, float F1()){
        *ptr_P = (F1()-*ptr_P)*2*t0 + *ptr_P;
        return *ptr_P;
    };
    
    float f8(float* ptr_P, float F1()){
        *ptr_P = (F1()-*ptr_P)*2*t0 + *ptr_P;
        return *ptr_P;
    };  
}

std::string getname(){
    return name;
};
std::string getaccount(){
    return account;
};

    void withdraw(std::string to_account, contract_asset amount)
    {
        int64_t account_id = get_account_id(to_account.c_str(), to_account.size());
        graphene_assert(account_id >= 0, "invalid account_name to_account");
        graphene_assert(amount.amount > 0, "invalid amount");

        uint64_t owner = get_trx_sender();
        auto it = accounts.find(owner);
        graphene_assert(it != accounts.end(), "owner has no asset");

        int asset_index = 0;
        for (auto asset_it = it->balances.begin(); asset_it != it->balances.end(); ++asset_it) {
            if ((amount.asset_id) == asset_it->asset_id) {
                graphene_assert(asset_it->amount >= amount.amount, "balance not enough");
                if (asset_it->amount == amount.amount) {
                    accounts.modify(it, 0, [&](auto &o) {
                        o.balances.erase(asset_it);
                    });
                    if (it->balances.size() == 0) {
                        accounts.erase(it);
                    }
                } else {
                    accounts.modify(it, 0, [&](auto &o) {
                        o.balances[asset_index] -= amount;
                    });
                }

                break;
            }
            asset_index++;
        }

        withdraw_asset(_self, account_id, amount.asset_id, amount.amount);
    }

int main(){
    users* iter;
    std::vector<users> userslist;
    std::vector<std::string> usersnamelist;
    time_t start_t, end_t, Start_t, End_t;
    int usersnum;
    int money;
    double diff_t;
    double a;
    int fm;
    double totalmoney=usersnum*money;
  
    for(int i=0, i<usersnum, i++){
        usersnamelist.push_back(getname());
    }
    for(int i=0, i<usersnum, i++){
        userslist.push_back(users(usersnamelist[i]));
    }
    for(int i=0, i<usersnum, i++){
        userslist[i].setaccount(getaccount());
    }
    print("Ready...Go!", "\n");
    time(&Start_t);
while(true){
    time(&start_t);
    for(int i=0, i<usersnum, i++){
        iter = &userslist[i];
        iter->f1();
        if(iter->P < iter->Pmo){
            if(iter->F1() > iter->P){
                if(iter->fjnow > iter->ffa){
                    iter->f2();
                }
                else{
                    iter->f3();
                }
            }
            else{
                if((iter->P-iter->Pcu)>0.75*(iter->Pmo - iter->Pcu)){
                    iter->f4();
                }
                else{
                    iter->f5();
                }
            }
            
        else{
            if(iter->F1() > iter->P){
                if(iter->fjnow > iter->ffa){
                    iter->f2();
                }
                else{
                    iter->f3();
                }
            }
            else{
                iter->f6();
            }
            }
        }
    }
    time(&End_t)
    if(difftime(End_t, Start_t)>90){
        break;
    }
    time(&end_t);
    diff_t = difftime(end_t, start_t);
    sleep(0.02-diff_t);
}
    for(int i=0, i<usersnum, i++){
        fm+=power((userslist[i]->wintime),a);
    }
if(fm>0){
    for(int i=0, i<usersnum, i++){
        userslist[i]->rate=pow((userslist[i]->wintime),a)/fm;
    }
}
else{
     withdraw(userslist[i]->account , totalmoney*0.95/usersnum);
      withdraw(cbny , totalmoney*0.05);
}
    if(fm>0){
        for(int i=0, i<usersnum, i++){
            withdraw(userslist[i]->account , userslist[i]->rate/fm*totalmoney);
        }
}
