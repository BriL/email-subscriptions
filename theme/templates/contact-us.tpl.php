<div class="grandera-contact-us container">
  <div class="row">
    <div class="col-md-6">
      <?php print render($form['first_name']); ?>
    </div>
    <div class="col-md-6">
      <?php print render($form['last_name']); ?>
    </div>
  </div>
  <div class="row">
    <div class="col-md-6">
      <?php print render($form['email']); ?>
    </div>
    <div class="col-md-6">
      <?php print render($form['reason']); ?>
    </div>
  </div>
  <div class="row">
    <div class="col-md-12">
      <?php print render($form['reason_description']); ?>
    </div>
  </div>
  <div class="row">
    <div class="col-md-12">
      <?php print render($form['submit']); ?>
    </div>
  </div>
    <?php print drupal_render_children($form); ?>
  </div>
</div>
