/**
 * @brief ラズパイ上のプロキシ
 * @file proxy.hpp
 */
#ifndef PROXY_HPP
#define PROXY_HPP

/**
 *socket通信の初期化
 * @return 通信に成功:true
 * @return 通信に失敗:false
 */
bool initialize();

/**
 *blockおよびnonceの中継
 */
void proxy_core();

/**
 *サーバとのコネクションの終了
 * @return コネクションの終了が成功:true
 * @return コネクションの終了が失敗:false
 */
bool finalize();

#endif
