<div class="wallet-info">
  <?php 
  $text = t('Wallet balance: !balance', [ '!balance'=> $gf_wallet_balance]);
  print($text); 
  ?>
</div>

<div class="update-balance-button"><?php print($update_wallet_button); ?></div>

