<?php

/**
 * Callback для страницы кошелька.
 */
function gf_wallet_user_wallet_page($user) {
  drupal_set_title(t("!user's wallet", ['!user'=>$user->name]));
  return theme('gf_wallet_user_wallet_page', ['account' => $user]);
}


/**
 * Обновление баланса кошелька посредством ajax с помощью ctools.
 */
function gf_wallet_update_balance_js($js=NULL, $account){
  global $user;
  $commands = [];
  $msg = t('You do not have access to this feature');

  if (user_access('administer gf-wallet')) {
    $form_state = array(
      'title' => t('Update balance'), 
      'ajax' => TRUE,
    );
    $form_state['build_info']['args'] = array($account);
    $commands = ctools_modal_form_wrapper('gf_wallet_balance_edit_form', $form_state);
    if (!empty($form_state['executed'])) {
      $commands[] = ajax_command_invoke('.wallet-balance-value', 'text', [$form_state['values']['balance']]);
      // Закрываем модальное окно.
      $commands[] = ctools_modal_command_dismiss();
    }
    return array('#type' => 'ajax', '#commands' => $commands);

  }
  else if ($user->uid == $account->uid) {
    $msg = t('To replenish your wallet, please contact your personal manager or sent us mail: !mail', 
      ['!mail'=>'<a href="mailto:info@giorgio-ferretti.it" target="_blank" rel="nofollow">info@giorgio-ferretti.it</a>']);
  }

  $commands[] = ajax_command_html('#modal-content', $msg);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Создание кнопки обновления баланса для использование ее с ctools.
 */
function gf_wallet_make_update_balance_button($uid) {
  static $added = FALSE;
  if (!$added) {
    $added = TRUE;
    // Подключаем нужные компоненты CTools.
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();

    // Создаем массив с настройками для модального окна.
    $style = [
      'gf-wallet-modal-style' => [
        'modalSize' => [
          'type' => 'fixed', // Тип модального окна. фиксированный или резиновый.
          'width' => 350, // Ширина модального окна.
          'height' => 'auto', // Высота модального окна.
        ],
        'modalOptions' => [
          'opacity' => (float) 0.3, // Прозрачность фона.
          'background-color' => '#000000', // Цвет фона.
        ],
        'closeText' => '', // Текст для закрытия модального окна.
        'loadingText' => '', // Текст, отображаемый в момент загрузки модального окна.
      ],
    ];

    // Включаем в контекст страницы настройки для модального окна.
    drupal_add_js($style, 'setting');
  }
  // Формирование ссылки.
  drupal_add_library('system', 'drupal.ajax');
  $dest = 'gf-wallet/nojs/update-balance/' . $uid;
  $data =  l(t('Top up balance'), $dest, [
    'html' => TRUE,
    'attributes' => [
      'class' => ['ctools-use-modal', 'ctools-modal-gf-wallet-modal-style', 'btn', 'btn-primary'],
      'title' => t('Click to update balance'),
    ],
  ]);

  return $data;
}
