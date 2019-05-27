#include "demo.hpp"

void demo::transfer(name from, name to, asset quantity, string memo)
{
    require_auth(from);
    if (from == _self || to != _self)
    {
        return;
    }
    if (memo == "i am boss")
    {
        return;
    }
    eosio_assert(quantity.amount <= 1000 * 10000, "you bet too high, buddy.");

    string gameMemo = GetGameMemo(from, memo);
    vector<string> v_head;
    SplitString(Memo_head, v_head, "-");
    vector<string> v_memo;
    SplitString(gameMemo, v_memo, ",");
    eosio_assert(v_memo.size() == 3, "invalid game memo.");

    print(gameMemo);
    
    // reward payment 
    pay_out(_self, name(v_head[0]), quantity, "You win!", name(v_head[1]));
}
