/-  *btc
|%
+$  host-info  [api-url=@t connected=? clients=(set ship)]
+$  req-id  @t
+$  command
  $%  [%set-credentials api-url=@t]
      [%whitelist-clients clients=(set ship)]
  ==
+$  action  [=req-id body=action-body]
+$  action-body
  $%  [%address-info =address]
      [%tx-info =txid]
      [%raw-tx =txid]
      [%create-raw-tx inputs=(list [=txid pos=@ud]) outputs=(list [=address value=sats])]
      [%ping ~]
  ==
+$  result  [=req-id body=result-body]
::  %raw-tx: result type for both %raw-tx and %create-raw-tx actions
::
+$  result-body
  $%  [%address-info utxos=(set utxo) used=? block=@ud]
      [%tx-info =info:tx]
      [%raw-tx =txid =rawtx]
  ==
+$  error
  $%  [%not-connected status=@ud]
      [%bad-request status=@ud]
      [%no-auth status=@ud]
      [%http-error status=@ud]
      [%rpc-error ~]
  ==
+$  update  (each result error)
+$  status
  $%  [%connected block=@ud fee=@ud]
      [%disconnected ~]
  ==
::
++  rpc
  |%
  +$  action

    $%  [%get-address-info =address]
        [%get-tx-vals =txid]
        [%get-raw-tx =txid]
        [%create-raw-tx inputs=(list [=txid pos=@ud]) outputs=(list [=address value=sats])]
        [%get-block-count ~]
        [%get-block-and-fee ~]
    ==
  ::
  +$  response
    $%  [%get-address-info utxos=(set utxo) used=? block=@ud]
        [%get-tx-vals =info:tx]
        [%get-raw-tx =txid =rawtx]
        [%create-raw-tx =rawtx]
        [%get-block-count block=@ud]
        [%get-block-and-fee block=@ud fee=sats]

    ==
  --
--
::
