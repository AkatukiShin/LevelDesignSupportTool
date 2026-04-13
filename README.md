# LevelDesignSupportTool

## 概要
レベル上で複数のアクターをまとめて選択し、位置・回転・スケールの固定オフセットやランダムなばらつきを一括で適用できるUE5用プラグインです。  

複数アクターに対して小物の散らし配置、向きのばらつき追加、スケール調整を素早く行えます。  
Previewで結果を確認してからApplyでき、Undo/Redoにも対応しています。  

## Supported Version
- Unreal Engine 5.7
- Editor Only
- Windows 動作確認済み

## Installation
1. このリポジトリをダウンロードします
2. `LevelDesignSupportTool` フォルダをプロジェクトの `Plugins` フォルダに配置します  
   （なければ作成してください）
3. Unreal Editor を起動します
4. Plugins ウィンドウから「LevelDesignSupportTool」を有効化します
5. Editor を再起動します

## Open Tool
ツールは以下の方法で起動できます：
- 上記のInstalationの手順を行う
- すると、下画像で赤く囲まれたボタンが出現する。
![UE5Display](https://github.com/user-attachments/assets/2300b038-a764-46a4-be8c-50ee5331f93c)
- ボタンを押すと次のようなWindowが立ち上がる。  
<img width="505" height="464" alt="スクリーンショット 2026-04-13 110659" src="https://github.com/user-attachments/assets/5eeee7e7-9729-49da-8549-4c7d56030071" >

## UI構成

ウィンドウは以下の構成になっています：

- Selected Actors  
  - 選択中アクターとクラスの一覧

- Preview  
  - 現在値 / 加算値 / 加算後の値を表示

- Transform Offset  
  - 固定値による位置・回転・スケールの加算

- Random Offset  
  - 範囲指定によるランダム加算

- Actions  
  - Preview / Apply / Reset Inputs

### SelectedActors
現在レベルで選択中のアクター一覧を表示します。
- アクター名
- クラス名
を確認できます。
一覧から対象を確認しながら作業が可能で、エディタ上の選択とも連動しています。

### Preview
- 現在値
- 加算値
- 加算後の値
を確認できます。
ランダムオフセットを使用している場合も、Previewで確認した内容をそのままApplyできるため、結果を見てから安心して確定できます。  
再度 Preview を押すと、ランダム値は再抽選されます。

### TransformOffset
固定値を使って、選択中アクターに一括でオフセットを加算します。
- 位置
- 回転
- スケール
それぞれを個別に設定できます。
ボタンによる増減にも対応しており、Step値を変更することで、ボタン操作時の増減量を調整できます。

### RandomOffset
最小値から最大値の範囲でランダム値を生成し、選択中アクターに一括で加算します。
- 位置
- 回転
- スケール
それぞれに対して、XYZごとに範囲指定できます。
各項目の左側が最小値、右側が最大値です。
ランダムな散らし配置や見た目のばらつき追加に向いています。

### Actions
操作用のボタンエリアです。
- Preview
- Apply
- Reset Inputs
を使って、確認・確定・入力値の初期化を行います。

## How to Use

1. レベル上でアクターを複数選択します
2. ツールを起動します
3. Selected Actors に対象が表示されます
4. TransformOffset または RandomOffset を設定します（CheckBoxで適用対象を選択できます）
5. 「Preview」を押して結果を確認します
6. 「Apply」を押して変更を適用します(Previewを押さない場合も変更を適応可能ですが、結果確認ができません)
7. 「Reset Inputs」で入力値を初期化できます

### CheckBox
各オフセット設定には CheckBox があり、チェックが入っている項目のみ変更が適用されます。

例：
- Location のみチェック → 位置のみ変更
- Rotation / Scale をオフ → 回転・スケールは変更されない

---

### Step
Step 値は、+ / - ボタンで増減する際の変化量を決定します。

例：
- LocationStep = 10 → ボタン1回で10ずつ移動
- RotationStep = 45 → 45度ずつ回転
