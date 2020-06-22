/**
 * @file lambda.h
 * @author Locietta
 * @brief �ú�ʵ��һ���򵥵�lambda���ʽ(��������)
 * @note �õ���GNU��չ����Ҫgcc���� "hack, but so far stable"
 * @bug �������ڶ��߳��첽
 * @version 0.1
 * @date 2020-06-13
 *
 */

#ifndef LAMBDA_H
#define LAMBDA_H

#define lambda(return_type, function_body)                                               \
    ({ return_type anonymous_func function_body anonymous_func; })

#endif // (!defined LAMBDA_H)
