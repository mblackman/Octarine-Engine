window.BENCHMARK_DATA = {
  "lastUpdate": 1778563691595,
  "repoUrl": "https://github.com/mblackman/Octarine-Engine",
  "entries": {
    "Octarine Engine Micro-Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "09244773702906313f383b89db5784b2fb322501",
          "message": "Update benchmark.yml",
          "timestamp": "2026-05-11T21:10:26-06:00",
          "tree_id": "54536db3ceb47b8cb58050f7f06289d29e46b930",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/09244773702906313f383b89db5784b2fb322501"
        },
        "date": 1778556609883,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3456.929890613707,
            "unit": "ns/iter",
            "extra": "iterations: 194968\ncpu: 3494.0109864185015 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 13648.756096175424,
            "unit": "ns/iter",
            "extra": "iterations: 50519\ncpu: 13692.249807002885 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 88290.2636313799,
            "unit": "ns/iter",
            "extra": "iterations: 7770\ncpu: 88335.26872587255 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 764075.3538860058,
            "unit": "ns/iter",
            "extra": "iterations: 929\ncpu: 764050.5382131365 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1553313.0319245094,
            "unit": "ns/iter",
            "extra": "iterations: 452\ncpu: 1553134.6349557445 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 1911.1642883828724,
            "unit": "ns/iter",
            "extra": "iterations: 372430\ncpu: 1884.8357248337854 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 4524.476659833194,
            "unit": "ns/iter",
            "extra": "iterations: 155857\ncpu: 4487.666861286538 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 25002.183391647046,
            "unit": "ns/iter",
            "extra": "iterations: 28027\ncpu: 24966.756912975674 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 190207.6695747927,
            "unit": "ns/iter",
            "extra": "iterations: 3675\ncpu: 190117.90530610576 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 402489.20603002654,
            "unit": "ns/iter",
            "extra": "iterations: 1731\ncpu: 402371.6545349249 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2676a5363e781349b5cec54a669c5e415a6c4763",
          "message": "Use a struct tag for projectiles",
          "timestamp": "2026-05-11T21:44:18-06:00",
          "tree_id": "5f11b79232c76aa84620f22d1219a7fb001d78c7",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2676a5363e781349b5cec54a669c5e415a6c4763"
        },
        "date": 1778558225934,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3818.09826108749,
            "unit": "ns/iter",
            "extra": "iterations: 180004\ncpu: 3857.914607453156 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15176.437546729761,
            "unit": "ns/iter",
            "extra": "iterations: 46710\ncpu: 15221.147377434287 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 101060.92939864348,
            "unit": "ns/iter",
            "extra": "iterations: 6887\ncpu: 101114.27646290151 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 859129.504003038,
            "unit": "ns/iter",
            "extra": "iterations: 823\ncpu: 858534.5540704767 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1734782.5906730343,
            "unit": "ns/iter",
            "extra": "iterations: 406\ncpu: 1734820.3497537198 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2232.837812036788,
            "unit": "ns/iter",
            "extra": "iterations: 318775\ncpu: 2202.2769069092205 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6491.464748230239,
            "unit": "ns/iter",
            "extra": "iterations: 108019\ncpu: 6456.151538157035 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40568.84696109123,
            "unit": "ns/iter",
            "extra": "iterations: 17371\ncpu: 40525.164066543446 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 311537.2657991693,
            "unit": "ns/iter",
            "extra": "iterations: 2244\ncpu: 311435.0762031487 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 645160.585531686,
            "unit": "ns/iter",
            "extra": "iterations: 1082\ncpu: 645001.7892791599 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "152f8e2e2337bbb79e8c74918afc9ec7b8490f6e",
          "message": "Fix projectiles masks spawned by the projectile emitter",
          "timestamp": "2026-05-11T22:25:47-06:00",
          "tree_id": "0edbd2d84978d058fd3bbc0e2ad5375cd6472018",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/152f8e2e2337bbb79e8c74918afc9ec7b8490f6e"
        },
        "date": 1778560363752,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3799.423877590467,
            "unit": "ns/iter",
            "extra": "iterations: 183066\ncpu: 3835.772595675822 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15245.192747241284,
            "unit": "ns/iter",
            "extra": "iterations: 45895\ncpu: 15288.657784070521 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102037.4697969949,
            "unit": "ns/iter",
            "extra": "iterations: 6862\ncpu: 102095.13538327305 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 853094.9696633022,
            "unit": "ns/iter",
            "extra": "iterations: 814\ncpu: 853087.41769042 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1737249.292960164,
            "unit": "ns/iter",
            "extra": "iterations: 403\ncpu: 1737349.4764268147 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2216.0882155504055,
            "unit": "ns/iter",
            "extra": "iterations: 330849\ncpu: 2173.9339154722006 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6492.077459859677,
            "unit": "ns/iter",
            "extra": "iterations: 108790\ncpu: 6439.404035293947 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40568.23403754909,
            "unit": "ns/iter",
            "extra": "iterations: 17291\ncpu: 40519.091550507444 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 313031.017949331,
            "unit": "ns/iter",
            "extra": "iterations: 2226\ncpu: 312924.5597484005 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653344.8041614469,
            "unit": "ns/iter",
            "extra": "iterations: 1069\ncpu: 653183.9569691714 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "ef5d49329d8b293539cb6c4a168adece6b2a0574",
          "message": "Update benchmark for what should be more consistent results",
          "timestamp": "2026-05-11T22:38:58-06:00",
          "tree_id": "14dff3d9af8b2516842bcf434b9bb7c07d377a5b",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/ef5d49329d8b293539cb6c4a168adece6b2a0574"
        },
        "date": 1778561143934,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3834.6458014707187,
            "unit": "ns/iter",
            "extra": "iterations: 178457\ncpu: 3869.841110183466 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15535.275013756045,
            "unit": "ns/iter",
            "extra": "iterations: 44644\ncpu: 15576.17906101668 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102246.82427595508,
            "unit": "ns/iter",
            "extra": "iterations: 6866\ncpu: 102281.77803670181 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 857785.212138862,
            "unit": "ns/iter",
            "extra": "iterations: 815\ncpu: 857857.3509202487 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1743814.4446234799,
            "unit": "ns/iter",
            "extra": "iterations: 402\ncpu: 1743966.0895522377 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2285.2716751964404,
            "unit": "ns/iter",
            "extra": "iterations: 311949\ncpu: 2249.7154086076775 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6728.110987665132,
            "unit": "ns/iter",
            "extra": "iterations: 105068\ncpu: 6688.48693227416 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 42291.23489716864,
            "unit": "ns/iter",
            "extra": "iterations: 16553\ncpu: 42253.935359154275 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 327032.3030145015,
            "unit": "ns/iter",
            "extra": "iterations: 2122\ncpu: 326930.978322309 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 678086.4162551977,
            "unit": "ns/iter",
            "extra": "iterations: 1037\ncpu: 677934.2748313035 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "de5709d23833ca5d9c6cbad913de7525f07e84b2",
          "message": "Render walking optimizations\n\nSingle-pass histogram — Build all 8 byte-histograms in one scan instead of 8 separate scans, eliminating ~3.5MB of redundant reads per sort\nSkip-uniform-byte passes — Detect bytes that are identical across all entries (e.g., shared layer) and skip those radix passes entirely. Cuts 8 passes to ~4 in the stress scene\nGather-based permutation — Replace the cycle-walk (random pointer chasing through 80-byte objects) with a sequential-write gather into a scratch buffer",
          "timestamp": "2026-05-11T22:59:01-06:00",
          "tree_id": "916e67143d9fcf476eb9317af1c9a56c4bdc2ea3",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/de5709d23833ca5d9c6cbad913de7525f07e84b2"
        },
        "date": 1778562354628,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3788.0753768039935,
            "unit": "ns/iter",
            "extra": "iterations: 183300\ncpu: 3827.5823349699567 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15348.905067644971,
            "unit": "ns/iter",
            "extra": "iterations: 45568\ncpu: 15393.833699087792 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 102655.79103891738,
            "unit": "ns/iter",
            "extra": "iterations: 6669\ncpu: 102719.63547757993 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 859713.6838588773,
            "unit": "ns/iter",
            "extra": "iterations: 820\ncpu: 859814.3341463477 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1740621.3041534298,
            "unit": "ns/iter",
            "extra": "iterations: 401\ncpu: 1740683.4488778133 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2250.3189991363733,
            "unit": "ns/iter",
            "extra": "iterations: 316921\ncpu: 2208.437998744909 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6475.797324603175,
            "unit": "ns/iter",
            "extra": "iterations: 108091\ncpu: 6431.413919751903 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40906.116104617526,
            "unit": "ns/iter",
            "extra": "iterations: 17049\ncpu: 40843.14692944409 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 316219.2182000618,
            "unit": "ns/iter",
            "extra": "iterations: 2239\ncpu: 316080.52612775756 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 655022.1498813273,
            "unit": "ns/iter",
            "extra": "iterations: 1076\ncpu: 654858.7908921909 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2a557375ca25b71ebc508e7a1e694b1a64bf2631",
          "message": "Optimize Registry::Update and system instrumentation\n\nCameraFollowSystem — Scoped to CameraFollowComponent to avoid O(N) execution (fixed ~4.7ms bottleneck)\n\nGather Boxes — Parallelized collision data gathering using ParallelForEach\n\nQuery Counting — Added GetCount() to ComponentQuery for pre-allocating result vectors\n\nType Prettifying — Improved PrettifyTypeName to trim GCC/Clang leading length prefixes",
          "timestamp": "2026-05-11T23:18:23-06:00",
          "tree_id": "f9e6e49d9442b2255075ee70d57f80d7da9bd485",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2a557375ca25b71ebc508e7a1e694b1a64bf2631"
        },
        "date": 1778563513803,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_EntityCreateAndBlam/8",
            "value": 3794.658491014893,
            "unit": "ns/iter",
            "extra": "iterations: 181811\ncpu: 3826.0031791257106 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/64",
            "value": 15147.188883997018,
            "unit": "ns/iter",
            "extra": "iterations: 45159\ncpu: 15187.851170309064 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/512",
            "value": 101024.00431233429,
            "unit": "ns/iter",
            "extra": "iterations: 6856\ncpu: 101069.0689906671 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/4096",
            "value": 855859.8107915636,
            "unit": "ns/iter",
            "extra": "iterations: 818\ncpu: 855840.7665036592 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityCreateAndBlam/8192",
            "value": 1731923.6884353464,
            "unit": "ns/iter",
            "extra": "iterations: 401\ncpu: 1731987.2443889813 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8",
            "value": 2247.4643449060436,
            "unit": "ns/iter",
            "extra": "iterations: 317489\ncpu: 2214.678335313486 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/64",
            "value": 6496.071067302138,
            "unit": "ns/iter",
            "extra": "iterations: 108051\ncpu: 6459.897816769269 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/512",
            "value": 40860.93843188749,
            "unit": "ns/iter",
            "extra": "iterations: 17040\ncpu: 40828.28926056052 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/4096",
            "value": 313982.13541545125,
            "unit": "ns/iter",
            "extra": "iterations: 2231\ncpu: 313895.8946660788 ns\nthreads: 1"
          },
          {
            "name": "BM_EntityPoolSpawnAndPark/8192",
            "value": 653873.5875101001,
            "unit": "ns/iter",
            "extra": "iterations: 1080\ncpu: 653707.862037124 ns\nthreads: 1"
          }
        ]
      }
    ],
    "Octarine Engine Macro-Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "09244773702906313f383b89db5784b2fb322501",
          "message": "Update benchmark.yml",
          "timestamp": "2026-05-11T21:10:26-06:00",
          "tree_id": "54536db3ceb47b8cb58050f7f06289d29e46b930",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/09244773702906313f383b89db5784b2fb322501"
        },
        "date": 1778556785553,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.055,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 13.059,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 14.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 604"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.073,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 604"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.144,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 604"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.103,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.114,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.75,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.992,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.142,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.196,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Collision System Update [max]",
            "value": 1.381,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 3.879,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.735,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 5.47,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 3.883,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.738,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 5.474,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.325,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.391,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Sort [max]",
            "value": 2.481,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.707,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.171,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Draw [max]",
            "value": 4.556,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Present [p99]",
            "value": 6.753,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Render: Present [max]",
            "value": 11.758,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.775,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.524,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 19.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.255,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 291"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.308,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 291"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.316,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 291"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.108,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.396,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.223,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.292,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 4.852,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 5.696,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.724,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 0.827,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 1.088,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 1.228,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 0.928,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 0.999,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 1.049,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.003,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2676a5363e781349b5cec54a669c5e415a6c4763",
          "message": "Use a struct tag for projectiles",
          "timestamp": "2026-05-11T21:44:18-06:00",
          "tree_id": "5f11b79232c76aa84620f22d1219a7fb001d78c7",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2676a5363e781349b5cec54a669c5e415a6c4763"
        },
        "date": 1778558400898,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 12.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 13.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.39,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.061,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 1.915,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 2.322,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 2.562,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 602"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 602"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 602"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.232,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.555,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.418,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.473,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 0.945,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.867,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.104,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.117,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 0.876,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.918,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.591,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.961,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.058,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Collision System Update [max]",
            "value": 1.655,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 0.965,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 1.659,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 302"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.589,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 3.921,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 4.811,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 589"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 1.107,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.155,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 4.868,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 583"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.435,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 8.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 9.865,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.44,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 8.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 9.871,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.357,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.466,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Sort [max]",
            "value": 2.306,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.892,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.247,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.309,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.383,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Present [p99]",
            "value": 6.908,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Render: Present [max]",
            "value": 12.229,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.972,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.606,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 18.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 301"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.723,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.432,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.901,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.212,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.239,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.245,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 5.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 5.728,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 5.915,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 295"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.245,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 289"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.386,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 289"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.473,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 289"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 294"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 294"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 294"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "152f8e2e2337bbb79e8c74918afc9ec7b8490f6e",
          "message": "Fix projectiles masks spawned by the projectile emitter",
          "timestamp": "2026-05-11T22:25:47-06:00",
          "tree_id": "0edbd2d84978d058fd3bbc0e2ad5375cd6472018",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/152f8e2e2337bbb79e8c74918afc9ec7b8490f6e"
        },
        "date": 1778560536006,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.014,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.041,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 12.062,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 13.072,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 16.126,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.056,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 1.805,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 2.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 2.508,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 606"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 606"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 606"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.229,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.262,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.38,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.011,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.414,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.467,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 2.279,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.235,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 1.389,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.244,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 1.416,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.946,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.082,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 2.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 0.995,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.135,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 1.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 306"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.139,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 306"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 2.145,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 306"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.403,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 3.923,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 5.609,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 5.893,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 590"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 1.042,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 2.318,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 586"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.067,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 7.469,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 20.577,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.071,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 7.475,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 20.623,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.373,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.451,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.652,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.499,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.244,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.284,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Present [p99]",
            "value": 6.77,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Render: Present [max]",
            "value": 6.95,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.484,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.124,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 10.564,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 303"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.168,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.884,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.038,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.079,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.131,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.215,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.235,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.32,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 5.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 5.561,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 5.775,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 297"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.235,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 293"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.367,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 293"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.384,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 293"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 296"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 288"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "ef5d49329d8b293539cb6c4a168adece6b2a0574",
          "message": "Update benchmark for what should be more consistent results",
          "timestamp": "2026-05-11T22:38:58-06:00",
          "tree_id": "14dff3d9af8b2516842bcf434b9bb7c07d377a5b",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/ef5d49329d8b293539cb6c4a168adece6b2a0574"
        },
        "date": 1778561321063,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.861,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 5.089,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 5.401,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 1.113,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.265,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 1.33,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 723"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.029,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.052,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.066,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.887,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.213,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.267,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 1.064,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 7.094,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 8.121,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 8.792,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.674,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.323,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Render: Present [max]",
            "value": 8.863,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 10.154,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 11.251,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 13.761,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.018,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 1.069,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 2.074,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 2.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 0.84,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 0.978,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 1.23,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.03,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 722"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.039,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 722"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.051,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 722"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.203,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.23,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.442,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.435,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.568,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 6.343,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.097,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.13,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 4.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.105,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 4.294,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.486,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.561,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.682,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.962,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.345,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.421,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.45,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.948,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.183,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 1.454,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.954,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 2.189,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 362"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.153,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.205,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.24,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.71,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 9.644,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 18.462,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.715,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 9.652,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 18.467,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.353,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.552,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Sort [max]",
            "value": 2.017,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.644,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.446,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 361"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "de5709d23833ca5d9c6cbad913de7525f07e84b2",
          "message": "Render walking optimizations\n\nSingle-pass histogram — Build all 8 byte-histograms in one scan instead of 8 separate scans, eliminating ~3.5MB of redundant reads per sort\nSkip-uniform-byte passes — Detect bytes that are identical across all entries (e.g., shared layer) and skip those radix passes entirely. Cuts 8 passes to ~4 in the stress scene\nGather-based permutation — Replace the cycle-walk (random pointer chasing through 80-byte objects) with a sequential-write gather into a scratch buffer",
          "timestamp": "2026-05-11T22:59:01-06:00",
          "tree_id": "916e67143d9fcf476eb9317af1c9a56c4bdc2ea3",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/de5709d23833ca5d9c6cbad913de7525f07e84b2"
        },
        "date": 1778562531582,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Render: Sort [p95]",
            "value": 0.943,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.206,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Sort [max]",
            "value": 3.668,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 3.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.337,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Draw [max]",
            "value": 4.369,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "16RenderTextSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "16RenderTextSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "16RenderTextSystem [max]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18CameraFollowSystem [p95]",
            "value": 3.586,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18CameraFollowSystem [p99]",
            "value": 3.868,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18CameraFollowSystem [max]",
            "value": 5.256,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18RenderSpriteSystem [p95]",
            "value": 0.995,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18RenderSpriteSystem [p99]",
            "value": 1.159,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "18RenderSpriteSystem [max]",
            "value": 1.618,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "19DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "19DisplayHealthSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "19DisplayHealthSystem [max]",
            "value": 0.01,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "21RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "21RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "21RenderPrimitiveSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 727"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.016,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.04,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.66,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.12,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.243,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.799,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.261,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.289,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.452,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Query::Update [max]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 7.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 8.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 11.542,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.334,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.078,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Render: Present [max]",
            "value": 9.063,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.755,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.673,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 17.248,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.021,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.024,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 2.075,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 2.618,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 3.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "12ScriptSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "12ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "12ScriptSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15AnimationSystem [p99]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "20ProjectileEmitSystem [p95]",
            "value": 0.775,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "20ProjectileEmitSystem [p99]",
            "value": 0.929,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "20ProjectileEmitSystem [max]",
            "value": 0.992,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.034,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 726"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.045,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 726"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.065,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 726"
          },
          {
            "name": "25ProjectileLifecycleSystem [p95]",
            "value": 0.202,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "25ProjectileLifecycleSystem [p99]",
            "value": 0.228,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "25ProjectileLifecycleSystem [max]",
            "value": 0.262,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "21KeyboardControlSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "21KeyboardControlSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "21KeyboardControlSystem [max]",
            "value": 0.009,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "14MovementSystem [p95]",
            "value": 0.434,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "14MovementSystem [p99]",
            "value": 0.59,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "14MovementSystem [max]",
            "value": 0.802,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.101,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.125,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.137,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15TransformSystem [p99]",
            "value": 0.136,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15TransformSystem [max]",
            "value": 0.15,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.46,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.524,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.612,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Gather Boxes [p95]",
            "value": 0.972,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.102,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.681,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.431,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 1.641,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.176,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "15CollisionSystem [p95]",
            "value": 1.436,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "15CollisionSystem [p99]",
            "value": 1.647,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "15CollisionSystem [max]",
            "value": 2.18,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 364"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.192,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 2.866,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 7.259,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 7.977,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 13.143,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 7.263,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 7.981,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 13.156,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 363"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "committer": {
            "email": "mblackman@users.noreply.github.com",
            "name": "mblackman",
            "username": "mblackman"
          },
          "distinct": true,
          "id": "2a557375ca25b71ebc508e7a1e694b1a64bf2631",
          "message": "Optimize Registry::Update and system instrumentation\n\nCameraFollowSystem — Scoped to CameraFollowComponent to avoid O(N) execution (fixed ~4.7ms bottleneck)\n\nGather Boxes — Parallelized collision data gathering using ParallelForEach\n\nQuery Counting — Added GetCount() to ComponentQuery for pre-allocating result vectors\n\nType Prettifying — Improved PrettifyTypeName to trim GCC/Clang leading length prefixes",
          "timestamp": "2026-05-11T23:18:23-06:00",
          "tree_id": "f9e6e49d9442b2255075ee70d57f80d7da9bd485",
          "url": "https://github.com/mblackman/Octarine-Engine/commit/2a557375ca25b71ebc508e7a1e694b1a64bf2631"
        },
        "date": 1778563691054,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Gather Boxes [p95]",
            "value": 1.212,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [p99]",
            "value": 1.605,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Gather Boxes [max]",
            "value": 1.864,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Collision System Update [p95]",
            "value": 1.675,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [p99]",
            "value": 2.088,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Collision System Update [max]",
            "value": 2.298,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CollisionSystem [p95]",
            "value": 1.682,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [p99]",
            "value": 2.092,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CollisionSystem [max]",
            "value": 2.302,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 384"
          },
          {
            "name": "CameraFollowSystem [p95]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "CameraFollowSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "CameraFollowSystem [max]",
            "value": 0.068,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 676"
          },
          {
            "name": "DisplayHealthSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "DisplayHealthSystem [p99]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "DisplayHealthSystem [max]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 687"
          },
          {
            "name": "RenderSpriteSystem [p95]",
            "value": 0.951,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderSpriteSystem [p99]",
            "value": 1.026,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderSpriteSystem [max]",
            "value": 1.16,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [p99]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderTextSystem [max]",
            "value": 0.008,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [p95]",
            "value": 0.004,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [p99]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "RenderPrimitiveSystem [max]",
            "value": 0.019,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 735"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p95]",
            "value": 0.112,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [p99]",
            "value": 0.164,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (pending blam/despawn) [max]",
            "value": 0.214,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p95]",
            "value": 4.174,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [p99]",
            "value": 4.592,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Registry::Update (total) [max]",
            "value": 5.185,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p95]",
            "value": 4.178,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [p99]",
            "value": 4.597,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Update (total) [max]",
            "value": 5.191,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p95]",
            "value": 1.025,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [p99]",
            "value": 1.14,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Sort [max]",
            "value": 1.462,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p95]",
            "value": 2.757,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [p99]",
            "value": 3.243,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Draw [max]",
            "value": 3.419,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p95]",
            "value": 6.369,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [p99]",
            "value": 7.148,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Render: Present [max]",
            "value": 7.932,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p95]",
            "value": 9.803,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [p99]",
            "value": 10.684,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::Render (total) [max]",
            "value": 12.013,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Game::ProcessInput [p95]",
            "value": 0.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [p99]",
            "value": 0.022,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::ProcessInput [max]",
            "value": 0.032,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p95]",
            "value": 0.02,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [p99]",
            "value": 0.033,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Bipartite [max]",
            "value": 0.047,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [p95]",
            "value": 0.093,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [p99]",
            "value": 0.132,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Brute Force Intersection [max]",
            "value": 0.17,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [p95]",
            "value": 0.25,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [p99]",
            "value": 0.268,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Partition Boxes [max]",
            "value": 0.316,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p95]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [p99]",
            "value": 0,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Query::Update [max]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p95]",
            "value": 6.571,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [p99]",
            "value": 7.015,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Sweep Bipartite [max]",
            "value": 7.306,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p95]",
            "value": 5.076,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [p99]",
            "value": 5.077,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "Game::WaitTime [max]",
            "value": 6.06,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "ScriptSystem [p95]",
            "value": 0.005,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [p99]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ScriptSystem [max]",
            "value": 0.007,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p95]",
            "value": 0.001,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [p99]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "AnimationSystem [max]",
            "value": 0.002,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p95]",
            "value": 0.893,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [p99]",
            "value": 0.957,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileEmitSystem [max]",
            "value": 0.972,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "CommandBuffer::Playback [p95]",
            "value": 0.037,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [p99]",
            "value": 0.048,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "CommandBuffer::Playback [max]",
            "value": 0.054,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 734"
          },
          {
            "name": "ProjectileLifecycleSystem [p95]",
            "value": 0.226,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [p99]",
            "value": 0.277,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "ProjectileLifecycleSystem [max]",
            "value": 0.309,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [p95]",
            "value": 0.006,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [p99]",
            "value": 0.012,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "KeyboardControlSystem [max]",
            "value": 0.044,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "MovementSystem [p95]",
            "value": 0.442,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "MovementSystem [p99]",
            "value": 0.61,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "MovementSystem [max]",
            "value": 0.629,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem: Fast [p95]",
            "value": 0.098,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [p99]",
            "value": 0.119,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem: Fast [max]",
            "value": 0.145,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 367"
          },
          {
            "name": "TransformSystem [p95]",
            "value": 0.11,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem [p99]",
            "value": 0.134,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "TransformSystem [max]",
            "value": 0.178,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 368"
          },
          {
            "name": "Emit Events [p95]",
            "value": 0.464,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [p99]",
            "value": 0.558,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          },
          {
            "name": "Emit Events [max]",
            "value": 0.653,
            "range": "0",
            "unit": "ms",
            "extra": "Samples: 366"
          }
        ]
      }
    ]
  }
}