/**
 * @file calcio.h
 * @author Locietta (https://github.com/Locietta)
 * @brief ���ڶ����һЩ�Զ��庯��
 * 
 * (defined CALCIO_H)
 * 
 * @version 0.1
 * @date 2020-05-10
 * 
 */

#ifndef CALCIO_H
#define CALCIO_H

#include <stdio.h>
#include <ctype.h>

/**
 * @brief Function: Expr_gets 
 * ����һ����ʽ,�ո��\t�ᱻ����,��д��ĸ�ᱻת��ΪСд��ĸ
 * 
 * @param[out] s ������ַ������������
 * @param[in] n ָ������ȡ����
 * @retval NULL �������/�����ļ���β
 * @retval �����ַ�ָ�� ����ɹ�,�����ַ�����׵�ַ
 * @par ʾ������
 * @code
 *  while (Expr_gets(expr, 100)) { 
 *      //TODO 
 *  }
 * @endcode
 * @{
 */
 char* Expr_gets(char *s, int n);
 /** @}*/


#endif // (!defined CALCIO_H)