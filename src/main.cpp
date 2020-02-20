#include <WiFi.h>
#include <Web3.h>
#include <Util.h>
#include <Contract.h>

const char *ssid = "Qryptos2";
const char *password = "Morrowind";
#define MY_ADDRESS "0x[WalletAddress]"      //Put your wallet address here
#define INFURA_HOST "kovan.infura.io"
#define INFURA_PATH "/v3/[Infura Key]"           //please change this Infura key to your own once you have the sample running
#define TARGETADDRESS "0xD9ba5B381d894956710fAb06e462D59e339Dd244"   //put your second address here
#define ETHERSCAN_TX "https://kovan.etherscan.io/tx/"

// Copy/paste the private key in here
#define PRIVATE_KEY "0x[PrivateKey]" //32 Byte Private key

Web3 web3(INFURA_HOST, INFURA_PATH);

void setupWifi();
void sendEthToAddress(double eths);

void setup() {
    Serial.begin(9600);
    setupWifi();
    sendEthToAddress(0.001);
}

void loop() {

}

void setupWifi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.persistent(false);
        WiFi.mode(WIFI_OFF);
        WiFi.mode(WIFI_STA);

        WiFi.begin(ssid, password);
    }

    int wificounter = 0;
    while (WiFi.status() != WL_CONNECTED && wificounter < 10)
    {
        for (int i = 0; i < 500; i++)
        {
            delay(1);
        }
        Serial.print(".");
        wificounter++;
    }

    if (wificounter >= 10)
    {
        Serial.println("Restarting ...");
        ESP.restart(); //targetting 8266 & Esp32 - you may need to replace this
    }

    delay(10);

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void sendEthToAddress(double eth)
{
    //obtain a contract object, for just sending eth doesn't need a contract address
    Contract contract(&web3, "");
    contract.SetPrivateKey(PRIVATE_KEY);
    unsigned long long gasPriceVal = 22000000000ULL;
    uint32_t  gasLimitVal = 4300000;
    string address = MY_ADDRESS;

    //convert eth value to Wei
    uint256_t weiValue = Util::ConvertEthToWei(eth);
    string emptyString = "";
    string destinationAddress = TARGETADDRESS;

    Serial.print("Get Nonc: ");
    uint32_t nonceVal = (uint32_t)web3.EthGetTransactionCount(&address);
    Serial.println(nonceVal);
    Serial.println("Send TX");
    string result = contract.SendTransaction(nonceVal, gasPriceVal, gasLimitVal, &destinationAddress, &weiValue, &emptyString);
    Serial.println(result.c_str());

    string transactionHash = web3.getString(&result);
    Serial.println("TX on Etherscan:");
    Serial.print(ETHERSCAN_TX);
    Serial.println(transactionHash.c_str()); //you can go straight to etherscan and see the pending transaction
}