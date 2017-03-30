<div class="trfs-email-container">
  <div class="row">
    <div class="col-md-12">
      <?php print $messages; ?>
      <a data-toggle="tooltip" data-placement="right" title="**By submitting your email you agree to receive personalized promotions and information from time to time. You may unsubscribe at anytime.**"><span class="glyphicon glyphicon-exclamation-sign"></span></a><?php print render($form['email']); ?><?php print render($form['submit']); ?>
    </div>  
  </div>
  <?php print drupal_render_children($form); ?>
</div>