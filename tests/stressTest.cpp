#include <atomic>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "cache/cacheConfig.h"
#include "cache/cacheEngine.h"
#include "eviction/LRUEvictionPolicy.h"
#include "protocol/CommandHandler.h"
#include "storage/HashMapStorage.h"

using namespace std;
using namespace cache;

int main()
{
    constexpr int kThreads = 16;
    constexpr int kOpsPerThread = 5000;

    CacheConfig config(1000);

    CacheEngine engine(
        config,
        make_unique<HashMapStorage>(),
        make_unique<LRUEvictionPolicy>()
    );

    CommandHandler handler(engine);

    atomic<uint64_t> opsCompleted{0};

    vector<thread> workers;

    for (int t = 0; t < kThreads; ++t)
    {
        workers.emplace_back([&, t]()
        {
            mt19937 rng(t);
            uniform_int_distribution<int> keyDist(0, 200);
            uniform_int_distribution<int> opDist(0, 4);

            for (int i = 0; i < kOpsPerThread; ++i)
            {
                string key = "key" + to_string(keyDist(rng));

                switch (opDist(rng))
                {
                    case 0:
                        handler.execute("SET " + key + " value" + to_string(i));
                        break;
                    case 1:
                        handler.execute("GET " + key);
                        break;
                    case 2:
                        handler.execute("DELETE " + key);
                        break;
                    case 3:
                        handler.execute("EXISTS " + key);
                        break;
                    case 4:
                        handler.execute("STATS");
                        break;
                }

                opsCompleted.fetch_add(1, memory_order_relaxed);
            }
        });
    }

    for (auto& w : workers) w.join();

    cout << "Completed " << opsCompleted.load()
         << " operations across " << kThreads << " threads.\n";
    cout << "Final cache size: " << engine.size() << "\n";

    auto stats = engine.getStats();
    cout << "Hits: " << stats.getHits()
         << ", Misses: " << stats.getMisses() << "\n";

    cout << "No crashes, no hangs, no ThreadSanitizer reports = PASS\n";

    return 0;
}
