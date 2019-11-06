#include <ipselockfund.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/system.hpp> 
#include <eosio/time.hpp>

ACTION ipselockfund::transfer(name from,name to,asset quantity,std::string memo) {
  eosio_assert(from != to, "cannot transfer to self");
  require_auth(from);
  eosio_assert(from == get_self(),"just ipselockfund could transfer token");
  eosio_assert(is_account(to),"to account does not exist");
  symbol symbol_ = symbol("POST", 4);
  lockfunds_index lockfunds("ipselockfund"_n,from.value);
  auto itr = lockfunds.find(to.value);
  if (itr != lockfunds.end()){
     std::string status = itr -> status;
     asset lockfund = itr -> lockfund;
     if (quantity >= lockfund){
       print("can not unlock asset over the lock fund");
       return;
     }
     if (status == "true"){
        action(
          permission_level{_self,"active"_n} ,
          "ipsecontract"_n,"transfer"_n,
          std::make_tuple(from,to,quantity,memo)
        ).send();
        uint32_t total_issue_times = itr -> total_issue_times;
        asset total_issue_asset = itr -> total_issue_asset;
        total_issue_asset = asset(total_issue_asset.amount + quantity.amount,symbol_);
        lockfunds.modify(itr,"ipselockfund"_n,[&]( auto& row){
          row.total_issue_times = total_issue_times + 1;
          row.total_issue_asset = total_issue_asset;
          row.last_issue_time = current_time();
        });
        require_recipient(from);
        require_recipient(to);
        print("transfer unlock fund success");
     }else{
       print("the staus of ipselockfund is not true");
     }
  }else{
    print("the to address does not lock fund");
  }
}

ACTION ipselockfund::changestatus(name address,std::string status){
  eosio_assert(address == "ipselockfund"_n,"change status must be ipselockfund");
  require_auth("ipselockfund"_n);
  lockfunds_index lockfunds("ipselockfund"_n,get_self().value);
  auto itr = lockfunds.find(address.value);
  if (itr != lockfunds.end()){
     lockfunds.modify(itr,"ipselockfund"_n,[&]( auto& row ){
        row.status = status;
      });
  }else{
    print("the to address does not lock fund");
  }
}


void ipselockfund::receiveLockfund(name from, name to, eosio::asset quantity, std::string memo){
  if (to != get_self() || from == get_self())
    return;
  if (from == "ipsefundteam"_n){
    print("ipse fund supplement balance");
    return;
  }
  auto sym = quantity.symbol;
  symbol symbol_ = symbol("POST", 4);
  eosio_assert(sym == symbol_,"just receive POST,please transfer POST to ipselockfund.");
  
  eosio_assert(quantity.amount > 5000000000,"transfer POST must > 500000");
  auto grade = "silver node";
  if (quantity.amount >= 80000000000){
    grade = "diamond node";
  }else if (quantity.amount >= 20000000000){
    grade = "gold node";
  }
  lockfunds_index lockfunds("ipselockfund"_n,to.value);
  auto itr = lockfunds.find(from.value);
  if(itr == lockfunds.end()){
    asset total_issue_asset = asset(0.0000,symbol_);
    lockfunds.emplace("ipselockfund"_n,[&](auto& row){
      row.address = from;
      row.from_name = memo;
      row.lockfund = quantity;
      row.timestamp = current_time();
      row.total_issue_times = 0;
      row.total_issue_asset = total_issue_asset;
      row.grade = grade;
      row.status = "false";
      row.last_issue_time = current_time();
    });
    print("you lock fund success");
  }else{
    print("the account is locked fund,please change an account to lock fund");
  }
}