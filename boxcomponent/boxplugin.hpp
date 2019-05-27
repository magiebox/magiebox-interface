#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

#define EOSIO_DISPATCH_BOX(TYPE, MEMBERS)                                                                                        \
    extern "C"                                                                                                                   \
    {                                                                                                                            \
        void apply(uint64_t receiver, uint64_t code, uint64_t action)                                                            \
        {                                                                                                                        \
            auto self = receiver;                                                                                                \
            bool self_action = code == self && action != name("transfer").value;                                                 \
            bool transfer = action == name("transfer").value;                                                                    \
            if (action == name("onerror").value)                                                                                 \
            {                                                                                                                    \
                /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
                eosio_assert(code == name("eosio").value, "onerror action's are only valid from the \"eosio\" system account");  \
            }                                                                                                                    \
            else if (self_action || transfer)                                                                                    \
            {                                                                                                                    \
                switch (action)                                                                                                  \
                {                                                                                                                \
                    EOSIO_DISPATCH_HELPER(TYPE, MEMBERS)                                                                         \
                }                                                                                                                \
            }                                                                                                                    \
        }                                                                                                                        \
    }

void SplitString(const string &s, vector<string> &v, const string &c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

string Memo_head = "no_head";
string GetGameMemo(name from, string memo)
{
    eosio_assert(from == "magiegateway"_n, "plase transfer to magiegateway first.");
    memo.erase(std::remove_if(memo.begin(),
                              memo.end(),
                              [](unsigned char x) { return std::isspace(x); }),
               memo.end());
    size_t sep_count_exit = std::count(memo.begin(), memo.end(), '>');
    vector<string> memo_v;
    SplitString(memo, memo_v, ">");
    eosio_assert(memo_v.size() == 2, "invalid game memo.");
    Memo_head = memo_v[0];
    return memo_v[1];
}

void pay_out(name from, name to, asset currency, string memo, name refferrer)
{
    name issuer;
    switch (currency.symbol.code().raw())
    {
    case (symbol("EOS", 4).code().raw()):
        issuer = "eosio.token"_n;
        break;
    case (symbol("GMC", 4).code().raw()):
        issuer = "gamechaineos"_n;
        break;
    case (symbol("COMC", 4).code().raw()):
        issuer = "community123"_n;
        break;
    default:
        eosio_assert(false, "invalid symbol.");
        break;
    }

    memo = to.to_string() + "-" + refferrer.to_string() + ">" + memo;
    action transfer_action = action(
        permission_level{from, "active"_n},
        issuer,
        "transfer"_n,
        std::make_tuple(from, "magiegateway"_n, currency, memo));
    transfer_action.send();
}