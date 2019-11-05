#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/name.hpp>

using namespace eosio;

CONTRACT ipselockfund : public contract {
  public:
    using contract::contract;
    ipselockfund(eosio::name receiver, eosio::name code, datastream<const char*> ds):contract(receiver, code, ds) {}

    ACTION transfer(name from,name to,asset quantity,std::string memo);
    
    ACTION changestatus(name address, std::string status);
    
    [[eosio::on_notify("ipsecontract::transfer")]]
    void receiveLockfund(name from,name to,asset quantity, std::string memo);
  
  private:
    TABLE lockfunds {
      name address;
      std::string from_name;
      asset lockfund;
      uint64_t timestamp;
      uint32_t total_issue_times;
      asset total_issue_asset;
      std::string grade;
      std::string status;
      uint64_t last_issue_time;
      uint64_t primary_key() const { return address.value;}
    };
    typedef eosio::multi_index<"lockfunds"_n, lockfunds> lockfunds_index;
};

EOSIO_DISPATCH(ipselockfund, (transfer)(changestatus))
