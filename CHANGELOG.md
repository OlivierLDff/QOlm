# Changelog

<a name="3.2.1"></a>
## 3.2.1 (2022-10-03)

### Changed

- 📌 googletest v1.12.1 [[cc2a3ac](https://github.com/OlivierLDff/QOlm/commit/cc2a3ac75c5ca9f4ca41365d01c32ed77f617f2c)]

### Miscellaneous

-  👷 Generate changelog from gitmoji [[bea1ad6](https://github.com/OlivierLDff/QOlm/commit/bea1ad69aca94a25e9d12900a9d5f08538c2e449)]
- 🔨 Only include GNUInstallDirs if required ([#15](https://github.com/OlivierLDff/QOlm/issues/15)) [[a60d280](https://github.com/OlivierLDff/QOlm/commit/a60d28080e0b862a8e6dcd74061ecb1e5a09969a)]
- 🔨 Only look for qt if not &#x60;Qt::Core&#x60; target exists ([#16](https://github.com/OlivierLDff/QOlm/issues/16)) [[3857c67](https://github.com/OlivierLDff/QOlm/commit/3857c6774dd6d5a65230fca56528b31a28ba046a)]
-  👷 qt 6.1.0 to qt 6.2.0 [[a31f50b](https://github.com/OlivierLDff/QOlm/commit/a31f50b32c81205db7da732090599d850121c5ca)]


<a name="3.2.0"></a>
## 3.2.0 (2021-10-11)

### Changed

- 📌 v3.2.0 [[d10c266](https://github.com/OlivierLDff/QOlm/commit/d10c266aadff4338abcdc17965261f28fa847446)]

### Miscellaneous

- 💥 uniformize objectRemoved signal with objectInserted &amp; objectMoved This make sure that the object isn&#x27;t present in list anymore when signal is emitted. This is way easier to deal with than about to be removed [[1cc7875](https://github.com/OlivierLDff/QOlm/commit/1cc7875ed99b3d6891461c85ae5fdb29d778ec2d)]
- 📝 Add CPM &amp; add_subdirectory install instruction [[c94ac30](https://github.com/OlivierLDff/QOlm/commit/c94ac306e874dedde114f8af97dd73e4aeae2c02)]
- 📝 Remove dependency note about eventpp [[fa53da0](https://github.com/OlivierLDff/QOlm/commit/fa53da017c367c003a5c6ac67d7c7ccba2b4eb8e)]


<a name="3.1.5"></a>
## 3.1.5 (2021-08-29)

### Changed

- 📌 v3.1.5 [[41daee3](https://github.com/OlivierLDff/QOlm/commit/41daee3ba5289be7ec839bade662f03dbc59b6ff)]

### Removed

- 🔇 Disable QOlm CMake log when project is included. Log can be enabled by user using QOLM_VERBOSE [[c711fdf](https://github.com/OlivierLDff/QOlm/commit/c711fdf34e60de686a2089885ad0f905c1c792d7)]

### Miscellaneous

- 🔨 Use standard CMAKE_INSTALL_INCLUDEDIR [[53e1cff](https://github.com/OlivierLDff/QOlm/commit/53e1cff7bd3928cbfefb017bc3b6d970b06a4ee3)]
- 🔨 Set global property USE_FOLDERS only if QOlm is main project [[ab92c69](https://github.com/OlivierLDff/QOlm/commit/ab92c698c13e21a92f287cb6436ee73378fa5ecd)]
- 🔨 Default build type to Release [[b584c4e](https://github.com/OlivierLDff/QOlm/commit/b584c4e915802cca52f036d69f0530b663fcecf3)]
- 🔨 Disallow in source build [[0d85845](https://github.com/OlivierLDff/QOlm/commit/0d85845e815a2aa80bc59cc97c58788f0f8a25fd)]
- 🔨 QOLM_VERBOSE if QOlm is main project [[417559f](https://github.com/OlivierLDff/QOlm/commit/417559f77d90b3ef731c660d73e2337c54bb56e9)]


<a name="3.1.4"></a>
## 3.1.4 (2021-07-15)

### Changed

- 📌 v3.1.4 [[0979401](https://github.com/OlivierLDff/QOlm/commit/0979401fc9c6ff719aa98533a4422d4e6f0026b1)]

### Fixed

- ✏️ typo [[ed8c800](https://github.com/OlivierLDff/QOlm/commit/ed8c800c2385f81866fa794ac4b1db77c11be857)]
- 🐛 Fix QOlmTargets.cmake export folder [[9b46bea](https://github.com/OlivierLDff/QOlm/commit/9b46bea9503926c0c0b1373c148c18012158d04f)]
- 🐛 Fix deployment of QOlm headers PUBLIC_HEADER strategy doesn&#x27;t work because it doesn&#x27;t keep file hierarchy Let&#x27;s just deploy &quot;by hand&quot; our header [[8198257](https://github.com/OlivierLDff/QOlm/commit/8198257f75a0b41d40675f47caa175a081e38e48)]

### Miscellaneous

- 📝 Update readme with QOlmConfig &amp; QOlmConfigVersion [[12f3c1a](https://github.com/OlivierLDff/QOlm/commit/12f3c1afe6239066e920b8cd56ca209e66e5a1c8)]
-  👷 Test that find_package work as expected with installed target [[f5d8de7](https://github.com/OlivierLDff/QOlm/commit/f5d8de79cdb922752f54e6c0cdb8d2131ee40b31)]
-  👷 Use emojies in build steps [[074a5c0](https://github.com/OlivierLDff/QOlm/commit/074a5c0c25c8d1af7d65dd9f5ae11199a32f6544)]
-  👷 We don&#x27;t need latest version of cmake anymore [[e2688c1](https://github.com/OlivierLDff/QOlm/commit/e2688c13ebdce4641287bd26cdc79695fc5137b5)]
- 🔨 Don&#x27;t install gtest with QOlm This allow to build with tests and still be able to install [[b341f9b](https://github.com/OlivierLDff/QOlm/commit/b341f9b28077ab3aa1c871d9c61cbce932d82eea)]
- 🔨 Deploy QOlmConfig.cmake information to be compatible with find_package [[845ce4b](https://github.com/OlivierLDff/QOlm/commit/845ce4bd751fff01b9408b502c61050971f619ed)]
- 🔨 Deploy QOlmConfigVersion to give version info [[5b68409](https://github.com/OlivierLDff/QOlm/commit/5b68409d3a447407368d32e1aac36374c832a1f6)]


<a name="3.1.3"></a>
## 3.1.3 (2021-07-15)

### Changed

- 📌 v3.1.3 [[724cabf](https://github.com/OlivierLDff/QOlm/commit/724cabf47a8ff4cc77321f2729a49bf56d75bae0)]

### Miscellaneous

- 💥 Rename QOLM_INSTALL to QOLM_ENABLE_INSTALL to keep consistency with other variable naming [[ecfc816](https://github.com/OlivierLDff/QOlm/commit/ecfc816cf2819afe681ee620564d7a339d63493d)]


<a name="3.1.2"></a>
## 3.1.2 (2021-07-15)

### Added

- 🔊 Add install command [[a33a13e](https://github.com/OlivierLDff/QOlm/commit/a33a13ed5461d32b126490ebddbd948c1e6d43b3)]
- 🔊 Add extra line return after build commands [[1874754](https://github.com/OlivierLDff/QOlm/commit/1874754df7d01a10782585d9b2524f2a847f5a01)]

### Changed

- 📌 v3.1.2 [[44c0e79](https://github.com/OlivierLDff/QOlm/commit/44c0e799e6d0b57a0ae455b632ef101bf34b87ef)]
- 🚚 Move QOlmBase.hpp &amp; Export.hpp to a Details folder This will create less confusing install folder with only QOlm.hpp [[de430f2](https://github.com/OlivierLDff/QOlm/commit/de430f27a179383be422766e91cd032781c34b2a)]

### Miscellaneous

-  👷 Run Ci only on push (remove pull_request) [[6eb3b45](https://github.com/OlivierLDff/QOlm/commit/6eb3b45e5fcae0233268ad50dfd9db0f470d8b6f)]
-  👷 rename matrix.version to matrix.qt-version for clearer code [[66b2d73](https://github.com/OlivierLDff/QOlm/commit/66b2d739edeb9c9c367d51c2f09227092085eebd)]
- 📝 Add info how to install QOlm [[0b7784b](https://github.com/OlivierLDff/QOlm/commit/0b7784b2112627881e559e2babe36328198733f7)]
- 📝 Fix missing include [[19ea657](https://github.com/OlivierLDff/QOlm/commit/19ea657250bcfbb614fab58b658b28327fa0ee63)]
- 📝 Fix QOlm is not header only anymore [[92cea84](https://github.com/OlivierLDff/QOlm/commit/92cea84dd001ebf8f92b735932960c2065541d73)]
- 📝 Use new cool namespace/alias library for cmake link example [[0d1a574](https://github.com/OlivierLDff/QOlm/commit/0d1a574e7d94e37091156becb3ea18e8fd39a677)]
- 💡 Section command in cmake [[bb77e9b](https://github.com/OlivierLDff/QOlm/commit/bb77e9b3b53dddee8d32446c7869bd3d0c053975)]
- 🔨 Introduce QOLM_INSTALL to make installation optional This option is enabled if QOlm is main project, and disabled if QOlm isn&#x27;t. It is user responsibility to enable it when fetching QOlm if he needs to install it [[9323b24](https://github.com/OlivierLDff/QOlm/commit/9323b24a2824f838fb45af2ff7bc3ec083cb717a)]
- 🔨 Add namespace name QOlm::QOlm [[60f4179](https://github.com/OlivierLDff/QOlm/commit/60f4179444ceea5b964e2eadd52b9dea07c0c238)]
- 🔨 Deploy QOlm header with QOlm installation [[3807e00](https://github.com/OlivierLDff/QOlm/commit/3807e00bdfa410fa7e62b93d74ccd8bc68e80e0f)]
- 🔨 Add installation support [[e006778](https://github.com/OlivierLDff/QOlm/commit/e0067783d443a305a11c3bf01916ef5a82cd33f2)]


<a name="3.1.1"></a>
## 3.1.1 (2021-05-23)

### Changed

- 📌 v3.1.1 [[5dbf06b](https://github.com/OlivierLDff/QOlm/commit/5dbf06b4c123ef6d4008372bf134c3c1e6a77b5f)]

### Miscellaneous

-  add CI configuration for Qt6 [[13a6cf9](https://github.com/OlivierLDff/QOlm/commit/13a6cf91ee21f3f558fcebf0f3b683e4f21018e9)]
-  support building with Qt6 [[10eeae2](https://github.com/OlivierLDff/QOlm/commit/10eeae2ffbeef68e510ac75b4398905fd4435d19)]
- 📝 Remove todo that were done [[557cdbb](https://github.com/OlivierLDff/QOlm/commit/557cdbbc44618b4214f3912286191c3e5ac64e21)]


<a name="3.1.0"></a>
## 3.1.0 (2021-04-12)

### Added

- ✨ Improve iterator API : cbegin, rbegin, crbegin [[ed6dad0](https://github.com/OlivierLDff/QOlm/commit/ed6dad0934cbd647ec854ae404aac7d3d91c50db)]
- 🔊 Fix log in indexOf saying object isn&#x27;t present. Index 0 exist, only index &lt; 0 doesn&#x27;t exist. [[9daa18a](https://github.com/OlivierLDff/QOlm/commit/9daa18a9d361909f5b36c1e58d6740474dbe8ded)]
- ✨ Make indexOf/remove/contains working with const QObject* [[b40037e](https://github.com/OlivierLDff/QOlm/commit/b40037e24b982fe2aec4d277cf54bfcc72f236f6)]

### Changed

- ♻️ Add full include path to qt include This add better overview of which qt library is used [[672132a](https://github.com/OlivierLDff/QOlm/commit/672132a1e32d8997db58cb2081dec5c271cc4d82)]

### Miscellaneous

- 📝 Copy from readme to docs [[c7e38e6](https://github.com/OlivierLDff/QOlm/commit/c7e38e6f953958de8ef4c0aad8b6dbf2d4ae1c86)]
-  👷 Fix missing libxcb-randr0 (ubuntu headless tests) [[5566653](https://github.com/OlivierLDff/QOlm/commit/5566653dbcd6ea08fb994fa1cf1ed28325d318d0)]
-  👷 Drop support for Qt5.12.x [[3a764c1](https://github.com/OlivierLDff/QOlm/commit/3a764c1136370028c20305d24ce7658b6f2c4beb)]
- 📄 CMakeLists license [[181ef0a](https://github.com/OlivierLDff/QOlm/commit/181ef0acccebf2b50b75d1050f8dbfe64878a795)]
-  👷 Update Ci to Qt5.15.2 [[9d4a565](https://github.com/OlivierLDff/QOlm/commit/9d4a56579cad06b919659f6926dfbc28285ed508)]
- 🔨 Print cmake configuration at the end of CMake [[b046060](https://github.com/OlivierLDff/QOlm/commit/b04606086fe6655d40512c58f3e4c5ae0d4bd25c)]
- 🔨 Use versionless qt version to prepare qt6 support (might work, not tested yet) [[b3bea0f](https://github.com/OlivierLDff/QOlm/commit/b3bea0f48becb60b2067bfcc6848af4bb2fdda92)]
- 📝 Add emojis to sections [[b1343e0](https://github.com/OlivierLDff/QOlm/commit/b1343e07591db391379fd39123dd6ad8282d7bf7)]
- 📝 Fix signal name [[a3af0f2](https://github.com/OlivierLDff/QOlm/commit/a3af0f276e6bd54a463b9f229c569410fe8151c1)]
- 📝 Documentation about new onInserted/onRemoved/onMoved callback [[d287d85](https://github.com/OlivierLDff/QOlm/commit/d287d8566794d34c5fa93b34b697c13091295656)]
- 💥 Remove eventpp api to replace it with qobject_cast and signal/lambda. It is way more standard to go that way within the qt world. And make it easier to manage connection &amp; lifetime [[24076a0](https://github.com/OlivierLDff/QOlm/commit/24076a0d2eca57220f21fb602beabbc276422963)]
- 📄 Add license to source files [[4a9f974](https://github.com/OlivierLDff/QOlm/commit/4a9f9744130a27659578957afd364fed0ae69f56)]
-  Update Readme.md [[21a3a2b](https://github.com/OlivierLDff/QOlm/commit/21a3a2b0ca594d1ec1a665fcf2c37afa60eabaf4)]
-  fix warning log [[c493077](https://github.com/OlivierLDff/QOlm/commit/c493077192dcf055c02a2ddc72c3ef7ab7d0c883)]
-  remove warning when clearing empty list [[4144b8c](https://github.com/OlivierLDff/QOlm/commit/4144b8c144c7841ee11936dbd6dfacb672d5b636)]
-  [Tests]: C++/Qml + CI [[23fe55b](https://github.com/OlivierLDff/QOlm/commit/23fe55bf3bd59ba8e2eb404260248e0d8de18b1d)]
-  fix eventpp branch [[2d842bf](https://github.com/OlivierLDff/QOlm/commit/2d842bf8680834facce0cd71c3843aae67df3a13)]
-  Update FetchGTest [[17d4ac4](https://github.com/OlivierLDff/QOlm/commit/17d4ac43df6e87c78d77bcbec7484c5fd1bb9b1c)]
-  Update Readme.md [[e265b92](https://github.com/OlivierLDff/QOlm/commit/e265b92ccbea9e1f52a8bbbc092251cd092db438)]
-  Update Readme.md [[e4b1f39](https://github.com/OlivierLDff/QOlm/commit/e4b1f39dd6504014ed92bf425c611ce80f54e04d)]
-  CI [[27e5a7c](https://github.com/OlivierLDff/QOlm/commit/27e5a7c17829aca209eee2c541ba9739dd7ca488)]
-  [QOlmBase]: Support build with version lower than Qt5.15 [[07d4fb6](https://github.com/OlivierLDff/QOlm/commit/07d4fb69d6d1d590a6daded93d788afaebc98b5b)]
-  [Model]: QOlmBase now take QJSValue as parameters. - Handle single object and array of objects - Handle a default children list - Example of how to use in qml [[3252b61](https://github.com/OlivierLDff/QOlm/commit/3252b6173f13f518f79a2be37738842bb8020054)]
-  [Model]: Update doc [[511f08f](https://github.com/OlivierLDff/QOlm/commit/511f08f9d1c21ca038f0c7be0d1b226beb340872)]
-  countChanged now emit the count to allow direct connection of the signal to a set&lt;Property&gt; function [[8937337](https://github.com/OlivierLDff/QOlm/commit/8937337d39e5b657f883759cc8e7a813fe882ff1)]


<a name="3.0.0"></a>
## 3.0.0 (2020-04-26)

### Miscellaneous

-  [CMake] : Fix get short commit command [[50562b5](https://github.com/OlivierLDff/QOlm/commit/50562b58e1ce730f9fa85d56e172ebbad0e8d5d0)]
-  Update namespace to qolm to avoid name conflict with class QOlm [[e9eb162](https://github.com/OlivierLDff/QOlm/commit/e9eb1629e9cceedb9bdfca96a69a8b255c374305)]
-  Fix missing typenames with gcc + fix QOlm_Example target name (it was QOlm_QOlmExample) [[26a313d](https://github.com/OlivierLDff/QOlm/commit/26a313d15167ea8acd98a66d4a2d6a27e606a914)]
-  Update .clang-format with new features [[c270aec](https://github.com/OlivierLDff/QOlm/commit/c270aec1dd5c495ea9345270f1b19a606c46531d)]
-  Simplify remove(_Object*) function by removing calling more api functions [[9af2e27](https://github.com/OlivierLDff/QOlm/commit/9af2e27028f6b96d336e00a7223b31c03690a22e)]
-  Add remove function that take a list of _Object* to remove multiple objects at once and add example [[4b32a41](https://github.com/OlivierLDff/QOlm/commit/4b32a41239216968d5d7ee9f48a632d5ce3a2f7f)]
-  Use .clang-format [[9f38006](https://github.com/OlivierLDff/QOlm/commit/9f38006e2d89294d1dab813c06d4bc171b02d912)]
-  QOlm refactor [[c8e459a](https://github.com/OlivierLDff/QOlm/commit/c8e459a38eec2e4ff6a03d18078702e949e6a273)]
-  [CMake] : use target_compile_features to set c++14 feature for Ereimote instead of setting global variable CMAKE_CXX_STANDARD [[51e03fc](https://github.com/OlivierLDff/QOlm/commit/51e03fc9108d457f4334bdd6420c14730c9d976e)]
-  some doc [[3059ce4](https://github.com/OlivierLDff/QOlm/commit/3059ce413620568f4ac1838e793713f4c70b55a8)]
-  remove useless space [[01a6e3c](https://github.com/OlivierLDff/QOlm/commit/01a6e3c1ddfb787c97441f7d02e4a41fe7980f7d)]
-  Update to 2.1.0 [[0cbc8eb](https://github.com/OlivierLDff/QOlm/commit/0cbc8eb1ea9ea3993fb0b0119c33e4ad4afdc0e8)]
-  Merge remote-tracking branch &#x27;origin/master&#x27; [[9788cae](https://github.com/OlivierLDff/QOlm/commit/9788cae656d7230c54bb017382789ba0fee51265)]
-  Remove useless and dangerous QSharedPointer (parent managed memory) [[d6eb1bb](https://github.com/OlivierLDff/QOlm/commit/d6eb1bb268d275ab0b23e821d79f965ec46facbf)]
-  minor clean [[d335347](https://github.com/OlivierLDff/QOlm/commit/d335347e2cd0039cc4741ef3ec4dee9d5d33cb28)]
-  Update Readme.md [[4bcc8be](https://github.com/OlivierLDff/QOlm/commit/4bcc8be9adad0b864f049ea3e2daee7600bff85c)]
-  Fix message STATUS [[303e0f0](https://github.com/OlivierLDff/QOlm/commit/303e0f03e55f77c9a23f19ef97429445e49b33b0)]
-  Better registration to qml [[e303a85](https://github.com/OlivierLDff/QOlm/commit/e303a85391db2271efc713fac1e79365d42632ba)]
-  Fix dependencies [[5f15050](https://github.com/OlivierLDff/QOlm/commit/5f1505028884118256f5b688e64f77919eee860d)]
-  Dependancies to Dependencies [[059d3f4](https://github.com/OlivierLDff/QOlm/commit/059d3f4543da9bdf538c14e9cd16e719fa63194e)]
-  Fix gtest build on windows by renaming gtest to googletest [[18c5dd9](https://github.com/OlivierLDff/QOlm/commit/18c5dd96a4571365a9a063e0284a40175fdee679)]
-  Fix DOBJLISTMODEL_DLL_EXPORT [[f6ec236](https://github.com/OlivierLDff/QOlm/commit/f6ec236e3f81bf9b0ac101c81a7746affdf2a543)]
-  configure gtest with gtest_force_shared_crt [[1063a3d](https://github.com/OlivierLDff/QOlm/commit/1063a3d70d755434e1af2a80ca0e92f08598cf7e)]
-  Fix qt dependencies [[f21f3c0](https://github.com/OlivierLDff/QOlm/commit/f21f3c04bf7058553cbe0889c7e59463e902d9ec)]
-  refractor [[899b12f](https://github.com/OlivierLDff/QOlm/commit/899b12f5bd1c7e7b4c7fd2e79f823fbfa199b7ac)]
-  remove dependency to Qt5Network [[93987c9](https://github.com/OlivierLDff/QOlm/commit/93987c969c77407dfea1150229ae184dc13b7617)]
-  Benchmark append [[3d1ecdb](https://github.com/OlivierLDff/QOlm/commit/3d1ecdbb48a74bc255e01587049e4094ac2e4ba1)]
-  Example + test signal in append function [[14e6fff](https://github.com/OlivierLDff/QOlm/commit/14e6ffffee893d5d170e2dbdd98ebabc150f52e6)]


<a name="2.0.0"></a>
## 2.0.0 (2019-09-03)

### Miscellaneous

-  hard fork from QQmlModel [[549b83d](https://github.com/OlivierLDff/QOlm/commit/549b83d48f13e3865037ab7d409f281ebee03a32)]


